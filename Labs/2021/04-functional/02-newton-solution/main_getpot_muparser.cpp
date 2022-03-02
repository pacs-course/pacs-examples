#include "muparser_fun.hpp"
#include "newton.hpp"

#include <GetPot>

int
main(int argc, char **argv)
{
  GetPot datafile("data.txt");

  std::string  fun_str   = datafile("fun", "x^3 + 5 * x + 3");
  std::string  prime_str = datafile("prime", "3 * x^2 + 5");
  muparser_fun fun(fun_str), prime(prime_str);


  int         n_max_it = datafile("n_max_it", 100);
  double      tol_fun  = datafile("tol_fun", 1.0e-14);
  double      tol_x    = datafile("tol_x", 1.0e-14);
  std::string filename = datafile("filename", "data.txt");

  newton_solver solver(fun, prime, n_max_it, tol_fun, tol_x);
  solver.solve(0.0);

  std::cout << "x    = " << solver.get_result() << std::endl;
  std::cout << "r    = " << solver.get_residual() << std::endl;
  std::cout << "iter = " << solver.get_iter() << std::endl;

  return 0;
}
