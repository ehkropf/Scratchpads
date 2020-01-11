#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <sstream>

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
/*
 * Some output stuff.
 */
#define STDOUT(S) std::cout << S

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
/*
 */

class SFormat
{
    static std::stringstream _buffer;
    static bool _used;

    void reset()
    {
        if (_used) _buffer.seekp(std::ios::beg);
        _used = true;
    }

public:
    /*
     * Conversion happens on construction.
     */
    template <class T>
    SFormat(const T& v)
    {
        reset();
        _buffer << v;
    }

    operator std::string() const { return _buffer.str().c_str(); }
};

std::stringstream SFormat::_buffer;
bool SFormat::_used = false;

class CSVline
{
    std::vector<std::string> _v;

public:
    CSVline() {}
    CSVline(const size_t n) : _v(n) {}

    const std::vector<std::string> vector() const { return _v; }

    std::string& operator[](size_t pos) { return _v[pos]; }
    const std::string& operator[](size_t pos) const { return _v[pos]; }

    std::string& back() { return _v.back(); }
    const std::string& back() const { return _v.back(); }
};

class CSVwriter
{
    std::unique_ptr<std::fstream> _pf;

    void check_open()
    {
        if (!_pf->is_open())
        {
            STDOUT("Failed to open CSV.\n");
        }
    }

public:
    /*
     * Vector type for writing.
     */
    using CSVvector = std::vector<std::string>;

    /*
     * Open with filename.
     */
    CSVwriter(const char* fname) : _pf(std::make_unique<std::fstream>(fname, std::ios::out))
    {
        check_open();
    }
    CSVwriter(const std::string& fname) : _pf(std::make_unique<std::fstream>(fname, std::ios::out))
    {
        check_open();
    }

    /*
     * Write vector of strings to line.
     */
    void writeLine(const CSVline& line)
    {
        const CSVvector& v = line.vector();
        *_pf << v[0];
        for (unsigned i = 1; i < v.size(); ++i)
        {
            *_pf << "," << v[i];
        }
        *_pf << "\n";
    }
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
/*
 * 2d equispaced grid.
 */
template <class T>
class Grid2d
{
    T _xmin, _xmax, _dx;
    T _tmin, _tmax, _dt;

    unsigned _nx;
    unsigned _nt;
    std::vector<T> _xpts;
    std::vector<T> _tpts;
    std::vector<T> _grid_array;

public:
    /*
     * Constructor requires x and t min/max, along with delta_x and delta_t.
     */
    Grid2d(T xmin, T xmax, T dx, T tmin, T tmax, T dt)
        : _xmin(xmin),
          _xmax(xmax),
          _dx(dx),
          _tmin(tmin),
          _tmax(tmax),
          _dt(dt)
    {
        _nx = std::trunc((xmax - xmin)/dx);
        _nt = std::trunc((tmax - tmin)/dt);
        _grid_array.resize(_nx*_nt);
    }

    /*
     * Access by (j,n) where 0 <= j <= nx and 0 <= n <= nt.
     */
    T& operator()(unsigned j, unsigned n)
    {
        return _grid_array[n*_nt + j];
    }
    const T& operator()(unsigned j, unsigned n) const
    {
        return _grid_array[n*_nt + j];
    }

    unsigned x_size() const { return _nx; }
    unsigned t_size() const { return _nt; }

    T x_val(unsigned j) const
    {
        if (j == 0) return _xmin;
        else if (j == _nx) return _xmax;
        else return j*_dx;
    }
    T t_val(unsigned n) const
    {
        if (n == 0) return _tmin;
        else if (n == _nt) return _tmax;
        else return n*_dt;
    }
};


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
/*
 * Hat function on [0,1].
 */
double hatfun(double x)
{
    if (x <= 0.5)
    {
        return 0.5*x;
    }
    return -0.5*x + 0.5;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int main(int argc, char** argv)
{
    /*
     * 1d heat equation explicit method.
     *
     * Desired: One run of explicit solver given $\delta x$ $\delta t$. Output
     * each step to file; each line is time step. Output file is CSV.
     *
     * Dreaming: Take function definition from external source.
     */

    if (argc < 3)
    {
        STDOUT("Missing arguments");
        return -1;
    }

    double dx{std::stod(argv[1])};
    double dt{std::stod(argv[2])};
    STDOUT("dx = " << dx << "; dt = " << dt << "\n");

    Grid2d<double> grid{0., 1., dx, 0., 1., dt};
    unsigned nx = grid.x_size();
    unsigned nt = grid.t_size();
    STDOUT("nx = " << nx << "; nt = " << nt << "\n");

    CSVwriter csv{"out.csv"};
    CSVline line{nx+1};

    // Set initial/boundary values here.
    line[0] = SFormat(grid.t_val(0));
    line[1] = line[0];
    line.back() = line[0];
    for (unsigned j = 1; j < nx-1; ++j)
    {
        double fv = hatfun(grid.x_val(j));
        grid(j,0) = fv;
        line[j+1] = SFormat(fv);
    }
    csv.writeLine(line);

    // mu = dt/dx^2 <= 1/2?
    double mu{dt/(dx*dx)};
    STDOUT("mu = " << mu << "\n");

    // Explicit method.
    auto Unext = [mu](Grid2d<double>& U, unsigned j, unsigned n)
    {
        return U(j,n) + mu*(U(j-1, n) - 2.0*U(j,n) + U(j+1,n));
    };

    line[1] = SFormat(0.0);
    line[nx] = SFormat(0.0);
    for (unsigned n = 1; n < nt; ++n)
    {
        line[0] = SFormat(grid.t_val(n));
        for (unsigned j = 1; j < nx-1; ++j)
        {
            double uval = Unext(grid, j, n-1);
            grid(j,n) = uval;
            line[j+1] = SFormat(uval);
        }
        csv.writeLine(line);
    }

    return 0;
}
