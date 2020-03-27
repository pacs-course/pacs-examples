#include "muparser_fun.h"
#include "newton.h"

#include <GetPot>

int
main(int argc, char **argv)
{
  GetPot g("data.txt");

  std::string  fun_str   = g("fun", "x^3+5*x+3");
  std::string  prime_str = g("prime", "3*x^2+5");
  muparser_fun fun(fun_str), prime(prime_str);


  int         n_max_it = g("n_max_it", 100);
  double      tol_fun  = g("tol_fun", 1.0e-14);
  double      tol_x    = g("tol_x", 1.0e-14);
  std::string filename = g("filename", "data.txt");

  newton_solver solver(fun, prime, n_max_it, tol_fun, tol_x);
  solver.solve(0.0);

  std::cout << "x    = " << solver.get_result() << std::endl;
  std::cout << "r    = " << solver.get_residual() << std::endl;
  std::cout << "iter = " << solver.get_iter() << std::endl;

  return 0;
}
