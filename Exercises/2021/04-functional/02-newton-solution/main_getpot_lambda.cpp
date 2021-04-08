#include "newton.hpp"

#include <GetPot>

int
main(int argc, char **argv)
{
  GetPot command_line(argc, argv);
  int    n_max_it = command_line("n_max_it", 100);
  double tol_fun  = command_line("tol_fun", 1.0e-16);
  double tol_x    = command_line("tol_x", 1.0e-16);

  auto fun = [](const double &x) -> double {
    return std::pow(x, 3) + 5 * x + 3;
  };
  auto prime = [](const double &x) -> double { return 3 * std::pow(x, 2) + 5; };

  newton_solver solver(fun, prime, n_max_it, tol_fun, tol_x);
  solver.solve(0.0);

  std::cout << "x =    " << solver.get_result() << std::endl;
  std::cout << "r =    " << solver.get_residual() << std::endl;
  std::cout << "iter = " << solver.get_iter() << std::endl;

  return 0;
}
