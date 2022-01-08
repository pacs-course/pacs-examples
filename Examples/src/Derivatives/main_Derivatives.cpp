#include "Derivatives.hpp"
#include "LambdaDerivatives.hpp"
#include <cmath>
#include <iostream>
int
main()
{
  std::function<double(const double &)> f = [](const double &x) {
    return std::pow(x, 4);
  };
  double h = 0.1;

  auto df = apsc::makeForwardDerivative<4>(f, h);
  std::cout << "Approx. fourth derivative of f=x^4 at x=5 is " << df(5.)
            << " Exact value " << 24 << std::endl;
  // but I can give any function object
  auto g = [](const double &x) { return std::exp(3*x); };
  std::cout
    << "Approx. third derivative of exp(3x) (backward formula) at x=2 is "
    << apsc::NthDerivative<3, decltype(g)>{g, h}(2.) << std::endl;
  std::cout << " The exact value is " << 27*std::exp(2.0*3.) << std::endl;
  std::cout << "Now I use a smaller h\n and a centered formula.\n";
  h = 1.e-4;
  auto finedf = apsc::makeCenteredDerivative<3>(g, h);
  std::cout << "Approx. third derivative of exp(3x) at x=2 is now "
            << finedf(2.0) << std::endl;
  std::cout << "Now the same with the lambda function.\n";
  std::cout << "Approx. third derivative of exp(3x) at x=2 with the lambda function "
             << apsc::lambda::derive<3>(g,2.0,h) << std::endl;



}
