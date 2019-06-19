#include <iostream>
#include <vector>
#include "boost/program_options.hpp"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#define SOUT(S) std::cout << S
#define SERR(S) std::cerr << S

constexpr double g_const = -9.8;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
struct Spring
{
    double length; // stretch length
    double k_const; // spring constant
    double mass; // mass on spring

    Spring(double length, double k_const, double mass)
        : length(length),
          k_const(k_const),
          mass(mass)
    {}

    /*
     * k/m = g/(scale*X)
     */
    static Spring fromRestPositionAndMass(double length, double rest_fraction, double mass)
    {
        double k = g_const * mass / (rest_fraction * -length);
        return Spring(length, k, mass);
    }
    static Spring fromRestPositionAndK(double length, double rest_fraction, double k_const)
    {
        double m = (k_const * rest_fraction * -length) / g_const;
        return Spring(length, k_const, m);
    }
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
namespace po = boost::program_options;

struct Options
{
    double X; // Spring stretch length (m).
    double rest_pos; // Position where total force on mass = 0.
    double x0; // Initial mass position x\in [0,X].
    unsigned N; // Number of timesteps.
    double T; // Time range t\in [0,T].

    double k_const; // Spring constant.
    double mass; // Mass on spring (g).

    enum State{OK, ERR, HELP};

    State state;

    // Options(double X, double rest_pos, double mass, double N, double T);
    Options(int argc, char** argv)
    {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("springstretch,X", po::value<double>(&X)->default_value(1.), "spring stretch length (m)")
            ("restpos,Z", po::value<double>(&rest_pos)->default_value(0.5), "zero force position [0,1]")
            ("initpos,x", po::value<double>(&x0)->default_value(0.8), "initial position [0,X]")
            ("nsteps,N", po::value<unsigned>(&N)->default_value(200), "number of time steps")
            ("timerange,T", po::value<double>(&T)->default_value(3), "time range (s)")
            ("springconst,k", po::value<double>(&k_const), "spring constant (N/m)")
            ("mass,m", po::value<double>(&mass), "mass (kg)");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help"))
        {
            SOUT(desc << "\n");
            state = State::HELP;
            return;
        }

        bool has_mass(vm.count("mass")), has_kconst(vm.count("k_const"));
        if (has_mass && !has_kconst)
        {
            mass = vm["mass"].as<double>();
            k_const = Spring::fromRestPositionAndMass(X, rest_pos, mass).k_const;
        }
        else if (!has_mass && has_kconst)
        {
            k_const = vm["k_const"].as<double>();
            mass = Spring::fromRestPositionAndK(X, rest_pos, k_const).mass;
        }
        else if (!(has_mass && has_kconst))
        {
            SERR("Must specify one or both of mass or spring constant.\n");
            SOUT(desc << "\n");
            state = State::ERR;
            return;
        }

        state = State::OK;
    }
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int main(int argc, char** argv)
{
    // Setup spring system. Should determine k or m given spring stretch length and resting length.
    // Determine number of time steps to calculate (given time step?).
    // Run forward Euler on problem.
    // Output to file.

    /*
     * System givem by total force: F_T = -kx + gm
     * where k is sprint constant, x is mass position in meters, g is gravity acceleration constant at
     * sea level (9.8 m/s^2) and m is mass in grams.
     */

    Options O(argc, argv);
    if (O.state == Options::State::HELP)
    {
        return 0;
    }
    else if (O.state == Options::State::ERR)
    {
        return -1;
    }

    double step = O.T/double(O.N-1);
    std::vector<double> t(O.N);
    for (unsigned i = 0; i < O.N; ++i)
    {
        t[i] = i*step;
    }

    std::vector<double> a(O.N, 0.), v(O.N, 0.), x(O.N, 0.);

    SERR("X = " << O.X << ", x_0 = " << -O.X*O.rest_pos << "\n");
    SERR("mass = " << O.mass << ", k = " << O.k_const << "\n");
    SERR("Time step size " << step << ", with t_0 = 0 and t_end = " << t[O.N-1] << "\n");

    // Euler's method.
    for (unsigned i = 0; i < O.N; ++i)
    {
        if (i > 0)
        {
            v[i] = v[i-1] + step * a[i-1];
            // x[i] = x[i-1] + step * v[i-1]; // Forward Euler is bad.
            x[i] = x[i-1] + step * v[i]; // Backward Euler is good.
        }
        else
        {
            x[0] = O.x0;
            v[0] = 0;
        }
        a[i] = -O.k_const*x[i]/O.mass + g_const;

        SOUT(" " << t[i] << "  " << x[i] << "  " << v[i] << "  " << a[i] << "\n");
    }

    return 0;
}
