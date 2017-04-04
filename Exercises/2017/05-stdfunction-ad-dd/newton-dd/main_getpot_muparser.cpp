#include "newton.h"
#include "muparser_fun.h"
#include <GetPot>

int
main (int argc, char ** argv)
{

  GetPot g (argc, argv);
  int maxit     = g ("maxit",  100);
  double funtol = g ("funtol", 1.0e-14);
  double xtol   = g ("funtol", 1.0e-14);
  std::string filename = g ("filename", "data.txt");

  GetPot g2 (filename.c_str ());
  std::string fun_str   = g2 ("fun", "x^3+5*x+3");
  muparser_fun fun (fun_str);
    
  newton_solver n(fun, maxit, funtol, xtol);
  n.solve (0.0);
  std::cout << "x    = " << n.get_result ()   << std::endl;
  std::cout << "r    = " << n.get_residual () << std::endl;
  std::cout << "iter = " << n.get_iter ()     << std::endl;

  return 0;
}
