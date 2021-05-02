#include <iostream>
#include <limits>
#include <iomanip>

//~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
int main()
{
    using dlimits = std::numeric_limits<double>;
    using flimits = std::numeric_limits<float>;

    std::cout << "type\t\tdigits\t\tdigits10\n"
        << "\nfloat\t\t" << flimits::digits << "\t\t" << flimits::digits10
        << "\ndouble\t\t" << dlimits::digits << "\t\t" << dlimits::digits10
        << "\n\n\t\tmin()\t\t\tlowest()\t\tmax()\n"
        << std::setprecision(flimits::digits10)
        << "\nfloat\t\t" << flimits::min() << "\t\t" << flimits::lowest() << "\t\t" << flimits::max()
        << std::setprecision(dlimits::digits10)
        << "\ndouble\t\t" << dlimits::min() << "\t" << dlimits::lowest() << "\t" << dlimits::max()
        << "\n\n\t\tepsilon()\t\tround_error()\t\tinfinity()\n"
        << std::setprecision(flimits::digits10)
        << "\nfloat\t\t" << flimits::epsilon() << "\t\t" << flimits::round_error() << "\t\t\t" << flimits::infinity()
        << std::setprecision(dlimits::digits10)
        << "\ndouble\t\t" << dlimits::epsilon() << "\t" << dlimits::round_error() << "\t\t\t" << dlimits::infinity()
        << "\n";

    std::cout << "\n\n\nvalue\t\tincrement\n\n";
    double eps{dlimits::epsilon()};
    double base{1.0};
    for (int i = 0; i < 10; ++i)
    {
        std::cout << base << "\t\t" << base*eps << "\n";
        base /= 10.0;
    }

    return 0;
}
