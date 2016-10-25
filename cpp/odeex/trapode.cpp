#include<iostream>
#include<iomanip>
#include<string>
#include<vector>
#include<armadillo>

typedef arma::Col<double> cvec;

int main()
{
    using namespace arma;

    std::string datafilename = "trapode.data";

    double h = 1.0/50.0;
    double tmax = 10.0;
    int numt = int(tmax/h) + 1;

    cvec tvec = cvec(numt, arma::fill::zeros);
    for (int i = 1; i < numt; ++i)
    {
        tvec(i) = tvec(i-1) + h;
    }

    cvec twot = 2.0*tvec;
    cvec yv = exp(-tvec)%sin(twot);
    cvec ypv = -yv + 2.0*exp(-tvec)%cos(twot);

    cvec sol = cvec(numt, fill::zeros);
    for (int i = 1; i < numt; ++i)
    {
        sol(i) = yv(i-1) + h*0.5*(ypv(i-1) + ypv(i));
    }

#define NOTFIRST span(1, numt-1)
    cvec err = abs(yv(NOTFIRST) - sol(NOTFIRST));
    cvec rerr = err/abs(yv(NOTFIRST));

//    std::cout << "error:\n" << err(span(0, 4))
//        << "\t.\n\t.\n\t.\n" << err(span(numt-6, numt-2)) << std::endl;
//    std::cout << "log of error:\n" << log(err(span(0, 4)))
//        << "\t.\n\t.\n\t.\n" << log(err(span(numt-6, numt-2))) << std::endl;
//    std::cout << "sol = \n" << sol(span(0, 4))
//        << "\t.\n\t.\n\t.\n" << sol(span(numt-5, numt-1)) << std::endl;
//    std::cout << "yv = \n" << yv(span(0, 4))
//        << "\t.\n\t.\n\t.\n" << yv(span(numt-5, numt-1)) << std::endl;

    std::ofstream dataf(datafilename, std::ios::out);
    dataf << "# time abserr relerr\n";
    dataf << "0.0 0.0 0.0\n";
    dataf << std::setprecision(6);

    for (int i = 0; i < numt-1; ++i)
    {
        dataf << tvec(i+1) << " "
            << err(i) << " "
            << rerr(i) << "\n";
    }
    dataf.close();

    return 0;
}
