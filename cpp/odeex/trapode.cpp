#include<iostream>
#include<vector>
#include<armadillo>

typedef arma::Col<double> cvec;

int main()
{
    double h = 1.0/50.0;
    double tmax = 10.0;
    int numt = int(tmax/h) + 1;

    cvec tvec = cvec(numt, arma::fill::zeros);
    for (int i = 1; i < numt; ++i)
    {
        tvec(i) = tvec(i-1) + h;
    }

    cvec twot = 2.0*tvec;
    cvec yv = arma::exp(-tvec)%arma::sin(twot);
    cvec ypv = -yv + 2.0*arma::exp(-tvec)%arma::cos(twot);

    cvec sol = cvec(numt, arma::fill::zeros);
    for (int i = 1; i < numt; ++i)
    {
        sol(i) = yv(i-1) + h*0.5*(ypv(i-1) + ypv(i));
    }

#define NOTFIRST arma::span(1, numt-1)
    cvec err = arma::abs(yv(NOTFIRST) - sol(NOTFIRST));
    cvec rerr = err/arma::abs(yv(NOTFIRST));
    std::cout << "error:\n" << err(arma::span(0, 4))
        << "\t.\n\t.\n\t.\n" << err(arma::span(numt-6, numt-2)) << std::endl;
    std::cout << "log of error:\n" << arma::log(err(arma::span(0, 4)))
        << "\t.\n\t.\n\t.\n" << arma::log(err(arma::span(numt-6, numt-2))) << std::endl;
//    std::cout << "sol = \n" << sol(arma::span(0, 4))
//        << "\t.\n\t.\n\t.\n" << sol(arma::span(numt-5, numt-1)) << std::endl;
//    std::cout << "yv = \n" << yv(arma::span(0, 4))
//        << "\t.\n\t.\n\t.\n" << yv(arma::span(numt-5, numt-1)) << std::endl;

    return 0;
}
