#include "newton.h"
#include <GetPot>
int
main (int argc, char ** argv)
{

  GetPot g (argc, argv);
  int maxit = g ("maxit", 100);
  double funtol = g ("funtol", 1.0e-16);
  double xtol = g ("xtol", 1.0e-16);

  auto fun   = [] (const double x)  -> double
    { return std::pow (x, 3) + 5 * x + 3; };
  auto prime = [] (const double x)  -> double
    { return 3 * std::pow (x, 2) + 5; };

  newton_solver n(fun, prime, maxit, funtol, xtol);
  n.solve (0.0);
  std::cout << "x =    " << n.get_result ()   << std::endl;
  std::cout << "r =    " << n.get_residual () << std::endl;
  std::cout << "iter = " << n.get_iter ()     << std::endl;
  return 0;

}
