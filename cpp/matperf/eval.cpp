#include <iostream>
#include <sstream>
#include <armadillo>

#include <SchwarzTypes.h>
#include <UnitCircleDomain.h>
#include <BoundaryValues.hpp>

#include "TimeThis.h"

using namespace ModifiedSchwarz;

//------------------------------------------------------------------------------
#define STDOUT(S) std::cout << S << std::endl
#define DOUT(S) std::cout << "<debug> " << S << std::endl

double MbFromByte(const double& b) { return b/(1024.*1024.); }

//------------------------------------------------------------------------------
cx_vec polesInHoles(const cx_vec& z, const UnitCircleDomain& D)
{
    cx_vec w(size(z), arma::fill::zeros);
    for (auto& d : D.centers()) w += 1./(z - d);

    return w;
}

//------------------------------------------------------------------------------
struct EvalVectors
{
    cx_vec svec;
    cx_vec hvec;

    EvalVectors(const ComplexBoundaryValues& v)
    {
        auto sigma = [](unsigned j) -> double { return j == 0 ? 1. : -1.; };
        const cx_mat& t_jk = v.points().matrix();
        const cx_mat& h_jk = v.values();
        unsigned N = t_jk.n_rows;
        const auto& D = v.domain();

        svec.set_size(t_jk.n_elem);
        hvec.set_size(t_jk.n_elem);

        for (unsigned j = 0; j < D.connectivity(); ++j)
        {
            auto s = arma::span(j*N, (j+1)*N - 1);
            svec.rows(s) = sigma(j) * (t_jk.col(j) - D.dv0(j));
            hvec.rows(s) = h_jk.col(j) % svec.rows(s);
        }
    }
};

//------------------------------------------------------------------------------
int main()
{
    std::cout.imbue(std::locale(""));
    constexpr unsigned double_sz = sizeof(double);
    DOUT("Size of double is " << double_sz << " bytes.");

    UnitCircleDomain D = domainExample3();
    ComplexBoundaryValues bv(
            BoundaryPoints(D),
            [&](const cx_vec& z) { return polesInHoles(z, D); });
    EvalVectors sumdat(bv);

    /*
     * Grid points for evaluation.
     */
    unsigned res = 500;
    cx_vec z = arma::vectorise(D.ngrid(res));
    z = z(find(D.inDomain(z)));
    DOUT("Setup " << z.n_elem << " points for evaluation -- "
            << MbFromByte(sizeof(z) + 2*z.n_elem*double_sz) << " Mb.");

    /*
     * Make Cauchy matrix for evaluation.
     */
    DOUT("Cauchy matrix storage " << bv.values().n_elem << "x" << z.n_elem << " -- "
            << MbFromByte(2*(bv.values().n_elem*z.n_elem)*double_sz) << " Mb.");
    cx_mat I;
    double secs = TimeThis([&]()
        {
            const cx_rvec&& t_jk = bv.points().vector().st();
            I = arma::repmat(t_jk, z.n_elem, 1);
            I.each_col() -= z;
            I = 1./I;
        }).seconds;
    DOUT("Matrix construction took " << secs << " seconds to build.");

//    STDOUT("Memory use size for points evaluated:");
//    STDOUT(" Points  | Memory (Mb)");
//    STDOUT("---------+------------");
//    unsigned step = 5e4;
//    unsigned p0 = 1e5;
//    for (unsigned i = 0; i < 10; ++i)
//    {
//        STDOUT(" " << p0 << " | " << MbFromByte(2*bv.values().n_elem*p0*double_sz));
//        p0 += step;
//    }

    /*
     * Evaluation -- two Cauchy sums and a matirx-element divide.
     */
    cx_vec w;
    secs = TimeThis([&]()
        {
            w = (I*sumdat.hvec)/(I*sumdat.svec);
        }).seconds;
    DOUT("Evaluation was " << secs << " seconds.");
    DOUT("Result w with " << w.n_elem << " points -- "
            << MbFromByte(sizeof(z) + 2*w.n_elem*double_sz) << " Mb.");

    return 0;
}
