#include "Derivatives.hpp"
#include <cmath>
#include <complex>
#include <functional>
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
  auto g = [](const double &x) { return std::exp(3 * x); };
  auto d3 = apsc::makeBackwardDerivative<3>(g, h);
  std::cout
    << "Approx. third derivative of exp(3x) (backward formula) at x=2 is "
    << d3(2.0) << std::endl;
  std::cout << " The exact value is " << 27 * std::exp(2.0 * 3.) << std::endl;
  std::cout << "Now I use a smaller h\n and a centered formula.\n";
  h = 1.e-4;
  auto finedf = apsc::makeCenteredDerivative<3>(g, h);
  std::cout << "Approx. third derivative of exp(3x) at x=2 is now "
            << finedf(2.0) << std::endl;
  std::cout << "Now the same with the lambda function.\n";
  auto dd3 = apsc::derive<3>(g, h);
  std::cout
    << "Approx. third derivative of exp(3x) at x=2 with the lambda function "
    << dd3(2.0) << std::endl;

  // it may be used also for complex functions!!
  using namespace std::complex_literals;
  auto fv = [](const std::complex<double> &x) {
    return x.real() * x.imag() + 1.i * x.imag() * x.imag();
  };
  auto dfx = apsc::derive<1>(fv, std::complex{h, 0.0});
  // auto dfy = apsc::derive<1>(fv,std::complex{0.0,h});
  auto dfy = apsc::makeCenteredDerivative<1>(fv, std::complex{0.0, h});
  std::cout << "With complex function x+iy-> x*y+iy^2: Derivative at 2+2i\n";
  std::complex z{2., 2.};
  // df/d(iy)= -i df/dy
  std::cout << "df/dx=" << dfx(z) << " -i df/dy=" << dfy(z) << std::endl;
}
