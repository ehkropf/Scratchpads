#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
template <class D>
D x_next_a(D xprev, D b, D c)
{
    D denom{-2.0/b};
    return -c/denom - xprev*xprev/denom;
}

template <class D>
D x_next_b(D xprev, D b, D c)
{
    return xprev - (xprev*xprev + 2.0*b*xprev + c)/(2.0*xprev + 2.0*b);
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
int main()
{
    constexpr double eps = std::numeric_limits<double>::epsilon();
    double c = 1.0;
    double b = -std::pow(10.0, 8.0);

    double xn = 0.0;
    auto xn1 = [b, c](double xn){ return x_next_b(xn, b, c); };
    double xprev = 0.0;
    std::cout << std::setprecision(std::numeric_limits<double>::digits10);
    std::cout
        << "b = " << b << "\n"
        << "c = " << c << "\n"
        << xn << "\n";
    do
    {
        xprev = xn;
        xn = xn1(xn);
        std::cout << xn << "\n";
    } while (std::fabs(xn - xprev) > 10.0*eps);

    return 0;
}
