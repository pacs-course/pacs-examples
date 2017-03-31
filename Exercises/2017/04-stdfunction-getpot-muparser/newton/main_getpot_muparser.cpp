#include "newton.h"
#include "muparser_fun.h"
#include <GetPot>

int
main (int argc, char ** argv)
{

  GetPot g (argc, argv);
  int maxit = g ("maxit", 100);
  double funtol = g ("funtol", 1.0e-16);
  double xtol = g ("funtol", 1.0e-16);

  std::string fun_str ("x*x*x+5*x+3");
  std::string prime_str ("3*x*x+5");
  muparser_fun fun (fun_str),
    prime (prime_str);
    
  newton_solver n(fun, prime, maxit, funtol, xtol);
  n.solve (0.0);
  std::cout << "x = " << n.get_result () << std::endl;
  std::cout << "r = " << n.get_residual () << std::endl;
  std::cout << "iter = " << n.get_iter () << std::endl;

  return 0;
}
