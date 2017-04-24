#include "newton.h"
#include <GetPot>

int
main (int argc, char ** argv)
{

  GetPot g (argc, argv);
  int maxit     = g ("maxit",  100);
  double funtol = g ("funtol", 1.0e-14);
  double xtol   = g ("xtol",   1.0e-14);
  std::string filename = g ("filename", "data.txt");


  var X, fun;
  fun = (X^3) + 1.5 * (X^2) - .5 * (X);

  std::cout << "first run, damping factor = 4.0" << std::endl;
  newton_solver n(fun, maxit, funtol, xtol, 4.0, 1.0e-10);
  n.solve (-1.0);
  std::cout << "x    = " << n.get_result ()   << std::endl;
  std::cout << "r    = " << n.get_residual () << std::endl;
  std::cout << "iter = " << n.get_iter ()     << std::endl;

  std::cout << std::endl;
  std::cout << std::endl;
    
  std::cout << "second run, damping factor = 2.0" << std::endl;
  newton_solver n2(fun, maxit, funtol, xtol, 2.0, 1.0e-10);
  n2.solve (-1.0);
  std::cout << "x    = " << n2.get_result ()   << std::endl;
  std::cout << "r    = " << n2.get_residual () << std::endl;
  std::cout << "iter = " << n2.get_iter ()     << std::endl;
  
  return 0;
}
