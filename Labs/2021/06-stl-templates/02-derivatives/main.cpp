#include "Derivatives.hpp"

#include <cmath>
#include <functional>
#include <iostream>

int
main(int argc, char **argv)
{
  const double h = 1e-2;

  const std::function<double(const double &)> f = [](const double &x) {
    return std::pow(x, 4);
  };

  const auto df = make_forward_derivative<4>(f, h);
  std::cout << "Approx. 4th derivative of x^4 at x = 5 is " << df(5.) << "."
            << std::endl;

  const auto g = [](const double &x) { return std::exp(x); };
  std::cout << "Approx. 3rd derivative of exp(x) at x = 2 is "
            << NthDerivative<3, decltype(g)>{g, h}(2.) << "." << std::endl;
}
