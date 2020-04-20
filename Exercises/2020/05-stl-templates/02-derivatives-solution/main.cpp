#include "Derivatives.hpp"

#include <cmath>
#include <iostream>

int
main()
{
  double h = 0.1;

  std::function<double(const double &)> f = [](const double &x) {
    return std::pow(x, 4);
  };

  auto df = make_forward_derivative<4>(f, h);
  std::cout << "Approx. 4th derivative of f at x = 5 is " << df(5.)
            << "." << std::endl;

  auto g = [](const double &x) { return std::exp(x); };
  std::cout << "Approx. 3rd derivative of exp(x) at x = 2 is "
            << NthDerivative<3, decltype(g)>{g, h}(2.) << "."
            << std::endl;
}
