#include <iostream>
#include <armadillo>

////////////////////////////////////////////////////////////////////////////////
#define OUT(S) std::cout << S << std::endl;

////////////////////////////////////////////////////////////////////////////////
const double pi = std::acos(-1.0);
const arma::cx_double i2pi(0.0, 2.0*pi);

// Make sample function interesting; put a couple of poles in the unit disk.
template <typename ArmaCxMat>
ArmaCxMat testFun(const ArmaCxMat& zv)
{
    const arma::cx_vec dv{
        arma::cx_double(-0.2517, 0.3129), arma::cx_double(0.2307, -0.4667) };
    ArmaCxMat wv = zv; // wv(zv.size(), arma::fill::zeros);
    for (const auto& d : dv) wv += 1/(zv - d);

    return wv;
}

////////////////////////////////////////////////////////////////////////
/*!
 * Vector polynomial evaluation.
 *
 * For n+1 coefficients in vector a and any x[i] in vector x, a degree
 * n polynomial over x is evaluated by
 *
 *     a[0]*x[i]^n + a[1]*x[i]^(n-1) + ... + a[n-1]*x[i] + a[n]
 *
 * using Horner's method.
 */
arma::cx_vec polyval(const arma::cx_vec& a, const arma::cx_vec& x)
{
    arma::cx_vec pn(arma::size(x), arma::fill::zeros);
    for (const auto& ai : a) pn = pn%x + ai;

    return pn;
}

/*!
 *  Trig interpolant. Pass in FFT array; constant and coefficients will
 *  be extracted. Points to evaluate should of course be on the unit circle.
 *  WARNING: Asumes size of FFT array is even!!!
 */
arma::cx_vec trigInterp(const arma::cx_vec& c, const arma::cx_vec& z)
{
    unsigned N = c.n_elem;

    const arma::cx_vec& pos_coeff(arma::flipud(c.rows(0, N/2)));
    arma::cx_vec pos_part(polyval(pos_coeff, z));

    const arma::cx_vec& neg_coeff(arma::join_vert(c.rows(N/2+1, N-1), arma::cx_vec{ 0. }));
    arma::cx_vec neg_part(polyval(neg_coeff, arma::cx_vec(1/z)));

    return pos_part + neg_part;
}

/*!
 *  Real-valued trig interpolant. Relies on c_k = conj(c_(-k)), and assumes
 *  FFT vector is full and even numbered.
 */
arma::colvec realTrigInterp(const arma::cx_vec& c, const arma::cx_vec& z)
{
    unsigned N = c.n_elem;

    const arma::cx_vec& coeff(arma::join_vert(arma::flipud(c.rows(1, N/2-1)),
                arma::cx_vec{ 0. }));
    return c(0).real() + 2.*arma::real(polyval(coeff, z));
}

////////////////////////////////////////////////////////////////////////////////
// Again it's assumed that c.n_elem is even.
void coeffOut(const arma::cx_vec& c)
{
    unsigned N = c.n_elem;

    OUT("Coeff (1,5) and (-1,-5):\n"
            << arma::join_horiz(c.rows(1, 5), flipud(c.rows(N-5, N-1)))
            <<"... (" << N/2-5 << ", " << N/2-1 << ") and ("
            << -int(N/2-5) << ", " << -int(N/2-1) << "): \n"
            << arma::join_horiz(c.rows(N/2-5, N/2-1), arma::flipud(c.rows(N/2+1, N/2+5)))
            );
}

////////////////////////////////////////////////////////////////////////////////
int main()
{
    // From the unit circle, take the N-point DFT of a given function.
    unsigned N = 128;
    arma::cx_vec unit_pts(arma::exp((arma::regspace(0, N-1)/N)*i2pi));
    arma::cx_vec w_pts(testFun(unit_pts));
    arma::cx_vec c(arma::fft(w_pts)/N);

    OUT("Constant (complex): " << c(0));
    OUT("Nyquist: " << c(N/2));
    coeffOut(c);

    // Take an odd/prime number of sample check points on the circle.
    unsigned Np = 13;
    arma::cx_vec sample_pts(arma::exp((arma::regspace(0, Np-1)/Np)*i2pi));
    arma::cx_vec sample_vals(testFun(sample_pts));

    // Use the trig interpolant to check values there.
    OUT("Check values in trig interpolant:\n"
            << arma::abs(sample_vals - trigInterp(c, sample_pts)));

    // Use the real part of the function only and take the 128-point DFT.
    arma::cx_vec rc(arma::fft(arma::real(w_pts))/N);

    // For DFT of real function, suppose N is even.
    // Then first coeff is real.
    // The remaining N-1 is odd, so we have Nyquist component at N/2.
    //
    // Arranging the coefficients (c_k) so we have negative indices, that is
    //     k in {N-(N/2-1),...,N-1} ~ k in {-(N/2-1),...,-1}
    // where c_0 is constant and c_(N/2) is nyquist component. We have the relation
    //     c_(k) = conj(c_(-k))
    // where conj() is the complex conjugate.
    OUT("Constant (real): " << rc(0));
    OUT("Nyquist: " << rc(N/2));
    coeffOut(rc);

    // Does the trig interpolant return good values at the check points?
    OUT("Value diff in real interpolant:\n"
            << arma::abs(arma::real(sample_vals) - realTrigInterp(rc, sample_pts)));

    return 0;
}
