#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
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
        _os.seekp(std::ios::beg);
        _newline = true;
    }
};

//------------------------------------------------------------------------------
/*
 * Test hat function
 */
double hatfun(double x)
{
    if (x < 0.5)
    {
        return 0.5*x;
    }
    return -0.5*x + 0.5;
}

//------------------------------------------------------------------------------
int main()
{
    std::string ofname{"out.csv"};
    size_t nx{101}, nt{10001};
    double dT = 0.5;

    double dx = 1.0/double(nx-1);
    double dt = dT/double(nt-1);
    STDOUT("dx = " << dx << ", dt = " << dt << "\n");
    double mu = dt/(dx*dx);
    STDOUT("mu = " << mu << "\n");

    auto ex_step = [mu](const Grid2d<double>& U, size_t j, size_t n)
    {
        return U(j,n-1) + mu*(U(j-1,n-1) - 2*U(j,n-1) + U(j+1,n-1));
    };

    Grid2d<double> grid(nx, nt);
    CSVwriter csv{ofname};

    // Initialize
    csv.add_entry("0,0");
    Grid2d<double>::iterator it = grid.xbegin(0);
    *it++ = 0.0;
    size_t j = 1;
    for (; it != grid.xend(0)-1; ++it)
    {
        *it = hatfun(j++/double(nx-1));
        csv.add_entry(*it);
    }
    *it = 0.0;
    csv.add_entry(0.0);
    csv.write_line();
    STDOUT("Initialized\n");

    // Solve over 1 sec.
    for (size_t n = 1; n < nt; ++n)
    {
        it = grid.xbegin(n);
        *it++ = 0.0;
        csv.add_entry(dT*(n/double(nt-1)));
        csv.add_entry(0.0);
        size_t j = 1;
        for (; it != grid.xend(n) - 1; ++it)
        {
            *it = ex_step(grid, j++, n);
            csv.add_entry(*it);
        }
        *it = 0.0;
        csv.add_entry(0.0);
        csv.write_line();
    }

    return 0;
}
