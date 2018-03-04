#include <iostream>
#include <armadillo>

#include "TimeThis.h"

//------------------------------------------------------------------------------
#define STDOUT(S) std::cout << S << std::endl

//------------------------------------------------------------------------------
const double pi = std::acos(-1);

//------------------------------------------------------------------------------
using cx_double = arma::cx_double;
using cx_vec = arma::cx_vec;
using cvec = arma::colvec;
using cx_mat = arma::cx_mat;

extern "C" void zfmm2dparttarg_(
        int* ier, int* iprec,
        int* nsource, cx_double* source, cx_double* dipstr,
        int* ifpot, cx_double* pot,
        int* ifgrad, cx_double* grad,
        int* ifhess, cx_double* hess,
        int* ntarget, cx_double* target,
        int* ifpottarg, cx_double* pottarg,
        int* ifgradtarg, cx_double* gradtarg,
        int* ifhesstarg, cx_double* hesstarg);

struct zFMM2d {
    // Input parameters.
    // Not including source, dipole strength, and target vectors.
    static int iprec;
    static int ifpot;
    static int ifgrad;
    static int ifhess;
    static int ifpottarg;
    static int ifgradtarg;
    static int ifhesstarg;

    // Output parameters.
    int ier;
    cx_vec pot;
    cx_vec grad;
    cx_vec hess;
    cx_vec pottarg;
    cx_vec gradtarg;
    cx_vec hesstarg;

    zFMM2d() = default;

    zFMM2d(cx_vec& source, cx_vec& dipstr, cx_vec& target)
    {
        int nsource = source.n_elem;
        int ntarget = target.n_elem;

        if (ifpot) pot.zeros(nsource);
        if (ifgrad) grad.zeros(nsource);
        if (ifhess) hess.zeros(nsource);
        if (ifpottarg) pottarg.zeros(ntarget);
        if (ifgradtarg) gradtarg.zeros(ntarget);
        if (ifhesstarg) hesstarg.zeros(ntarget);

        zfmm2dparttarg_(
                &ier, &iprec,
                &nsource, source.memptr(), dipstr.memptr(),
                &ifpot, pot.memptr(),
                &ifgrad, grad.memptr(),
                &ifhess, hess.memptr(),
                &ntarget, target.memptr(),
                &ifpottarg, pottarg.memptr(),
                &ifgradtarg, gradtarg.memptr(),
                &ifhesstarg, hesstarg.memptr());
    }
};

// Set defaults.
int zFMM2d::iprec = 5;
int zFMM2d::ifpot = false;
int zFMM2d::ifgrad = false;
int zFMM2d::ifhess = false;
int zFMM2d::ifpottarg = true;
int zFMM2d::ifgradtarg = false;
int zFMM2d::ifhesstarg = false;

//------------------------------------------------------------------------------
int main()
{
    int nsource = 800;
    int ntarget = 100000;

    cvec phi;

    phi.randu(nsource);
    phi *= 2.*pi;
    cx_vec source = arma::exp(cx_double(0., 1.)*phi);

    cx_vec dipstr = arma::ones<cx_vec>(nsource);

    STDOUT("source has " << source.n_elem << " points");

    phi.randu(ntarget);
    phi *= 2.*pi;
    cx_vec target = arma::exp(cx_double(0., 1.)*phi) + 2.;

    cx_vec pottarg(ntarget);

    STDOUT("target has " << target.n_elem << " points");

    // FMM!
    zFMM2d fmm; //(source, dipstr, target);
    auto dofmm = [&]() { fmm = zFMM2d(source, dipstr, target); };

    STDOUT("FMM took " << TimeThis(dofmm).seconds << " seconds");
    STDOUT("ier has " << fmm.ier);

    // Direct multiplication to check.
    cx_mat I;
    cx_vec result;
    auto domat = [&]() {
        I = arma::repmat(-source.st(), target.n_elem, 1);
        I.each_col() += target;
        I = 1./I;
        result = I*dipstr;
    };

    STDOUT("Matrix took " << TimeThis(domat).seconds << " seconds");
    STDOUT("Check matrix I is " << I.n_rows << "x" << I.n_cols);

    cvec relerr = arma::abs(result - fmm.pottarg)/abs(result.max());
    STDOUT("Max relerr is " << relerr.max());

    return 0;
}
