#include <iostream>
#include <armadillo>

int main()
{
  arma::mat A = arma::randu<arma::mat>(4,5);
  arma::mat B = arma::randu<arma::mat>(4,5);
  arma::mat C = arma::randu<arma::mat>(8,9);
  arma::mat D = arma::randu<arma::mat>(8,9);

  std::cout << A*B.t() << std::endl;
  std::cout << C*D.t() << std::endl;

  return 0;
}
