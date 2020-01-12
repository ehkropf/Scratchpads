#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <strstream>

using std::size_t;

//------------------------------------------------------------------------------
/*
 * I/O stuff
 */
#define STDOUT(S) std::cout << S

//------------------------------------------------------------------------------
/*
 * 2d grid class.
 *
 * Grid doesn't care about scale, only number of points in each dimension.
 * x-points are indexed by j, and t-points indexed by n.
 *
 */
template <class T>
class Grid2d
{
    std::vector<T> _v;
    size_t _nt, _nx;

public:
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    Grid2d() {}
    Grid2d(size_t nx, size_t nt) : _v(nx*nt), _nt(nt), _nx(nx) {}

    T& operator()(size_t j, size_t n) { return _v[_nx*n + j]; }
    const T& operator()(size_t j, size_t n) const { return _v[_nx*n + j]; }

    iterator xbegin(size_t n) { return _v.begin() + _nx*n; }
    iterator xend(size_t n) { return xbegin(n) + _nx; }
    const_iterator xbegin(size_t n) const { return _v.begin() + _nx*n; }
    const_iterator xend(size_t n) const { return xbegin(n) + _nx; }

    friend std::ostream& operator<<(std::ostream& os, const Grid2d<T>& grid)
    {
        size_t j = 0;
        for (auto it = grid._v.begin(); it != grid._v.end(); ++it)
        {
            if (j == 0) STDOUT(*it);
            else STDOUT(", " << *it);
            ++j;
            if (j == grid._nx)
            {
                j = 0;
                STDOUT("\n");
            }
        }

        return os;
    }
};

using DGrid2d = Grid2d<double>;

//------------------------------------------------------------------------------
/*
 * CSV writer
 */
class CSVwriter
{
    std::ofstream _f;

    std::ostrstream _os;
    bool _newline{true};

public:
    CSVwriter(const char* fname) { _f.open(fname); }
    CSVwriter(const std::string& fname) { _f.open(fname); }

    template <class T>
    void add_entry(const T& e)
    {
        if (!_newline)
        {
            _os << ",";
        }
        else
        {
            _newline = false;
        }
        _os << e;
    }

    void write_line()
    {
        _f << _os.str() << "\n";
        _os = std::ostrstream();
        _newline = true;
    }
};

//------------------------------------------------------------------------------
/*
 * Excplicit method for the 1d heat equation.
 *
 * Dirichlet boundary conditions -- U(0,0) = 0 and U(1, T) = 0 for t in [0,T].
 */
class ExplicitHeat
{
public:
    using InitFunction = std::function<double(double)>;

    struct Parameters
    {
        double dx, dt;
        size_t nx, nt;
        double dX{1.0}, dT{1.0};
        double mu;
        // Initial condition function.
        InitFunction U0;

        Parameters(size_t nx, size_t nt, InitFunction U0) : nx(nx), nt(nt), U0(U0)
        {
            dx = dX/double(nx-1);
            dt = dT/double(nt-1);
            set_mu();
        }

        void set_mu()
        {
            mu = dt/(dx*dx);
            if (mu > 0.5)
            {
                STDOUT("Warning: mu > 0.5!\n");
            }
        }

        double x_(size_t j) { return dX*(j/double(nx-1)); }
        double t_(size_t n) { return dT*(n/double(nt-1)); }
    };

private:
    Parameters _p;
    DGrid2d _grid;

protected:
    double step(size_t j, size_t n) const
    {
        const DGrid2d U = _grid;
        return U(j,n-1) + _p.mu*(U(j-1,n-1) - 2*U(j,n-1) + U(j+1,n-1));
    }

    void initialize()
    {
        STDOUT("Initializing...\n");
        DGrid2d::iterator it = _grid.xbegin(0);
        *it++ = 0.0;
        size_t j = 1;
        for (; it != _grid.xend(0)-1; ++it)
        {
            *it = _p.U0(j++/double(_p.nx-1));
        }
        *it = 0.0;
        STDOUT("Done\n");
    }

    void solve_grid()
    {
        STDOUT("Solving...\n");
        for (size_t n = 1; n < _p.nt; ++n)
        {
            DGrid2d::iterator it = _grid.xbegin(n);
            *it++ = 0.0;
            size_t j = 1;
            for (; it != _grid.xend(n)-1; ++it)
            {
                *it = step(j++, n);
            }
            *it = 0.0;
        }
        STDOUT("Done\n");
    }

public:
    ExplicitHeat(const Parameters& data) : _p(data), _grid{_p.nx, _p.nt}
    {
        initialize();
        solve_grid();
    }

    /*
     * Write file to be read by gnuplot.
     *
     * GNUplot requires each line be at least on pair of data points.
     *
     * Layout:
     *       t_0    t_1    ...   t_N
     * x_0   U_0,0  U_0,1
     * x_1   U_1,0  U_1,1
     *  .
     *  .
     *  .
     * x_J   U_J,0               U_J,N
     */
    void write_gnuplot(const std::string& fname)
    {
        std::ofstream of{fname};

        // Need to learn to implement iterators :P
        // Header row.
        of << "x_j/t_n";
        for (size_t n = 0; n < _p.nt; ++n)
        {
            of << " " << _p.t_(n);
        }
        of << "\n";

        // Each row is time evolution at x_j.
        for (size_t j = 0; j < _p.nx; ++j)
        {
            of << _p.x_(j);
            for (size_t n = 0; n < _p.nt; ++n)
            {
                of << " " << _grid(j,n);
            }
            of << "\n";
        }
    }
};

//------------------------------------------------------------------------------
int main()
{
    std::string ofname{"out.data"};

    //size_t nx{101}, nt{10001};
    //double dT = 0.5;
    size_t nx{11}, nt{201};
    //double dT = 1.0;

    // Initial condition function
    auto hatfun = [](double x)
    {
        if (x < 0.5)
        {
            return 0.5*x;
        }
        return -0.5*x + 0.5;
    };

    ExplicitHeat::Parameters D{nx, nt, hatfun};
    STDOUT("dx = " << D.dx << ", dt = " << D.dt << "\nmu = " << D.mu << "\n");

    ExplicitHeat H{D};
    H.write_gnuplot(ofname);

    return 0;
}
