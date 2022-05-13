#include <dlfcn.h>

#include <cmath>
#include <functional>
#include <iostream>

double
integrand(double x)
{
  return (std::pow(std::sin(std::pow(x, 2)), 2));
}

int
main(int argc, char **argv)
{
  const double res = integrate(integrand, 0, M_PI);

  std::cout << "res = " << res << std::endl;

  return 0;
}
