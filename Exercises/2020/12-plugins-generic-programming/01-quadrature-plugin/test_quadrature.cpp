#include <dlfcn.h>

#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>

double
integrand(double x)
{
  return (std::pow(std::sin(std::pow(x, 2)), 2));
}

int
main(int argc, char **argv)
{
  double pi  = 4 * atan(1);
  double res = integrate(integrand, 0, pi);

  std::cout << "res = " << res << std::endl;

  return 0;
}
