#include <iostream>
#include <armadillo>

int main()
{
    using namespace arma;

    arma_rng::set_seed_random();
    mat A(4, 3, fill::randu);

    std::cout << "Matrix\n" << A;
    std::cout << "Vectorized\n" << vectorise(A);

    return 0;
}
