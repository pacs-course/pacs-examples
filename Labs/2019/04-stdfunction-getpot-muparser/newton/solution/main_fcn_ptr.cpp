#include "newton.h"

double
fun (double x)
{ return std::pow (x, 3) + 5 * x + 3; };

double
prime (double x)
{ return 3 * std::pow (x, 2) + 5; };

int
main ()
{

  newton_solver n(fun, prime);
  n.solve (0.0);

  std::cout << "x =    " << n.get_result ()   << std::endl;
  std::cout << "r =    " << n.get_residual () << std::endl;
  std::cout << "iter = " << n.get_iter ()     << std::endl;

  return 0;
}
