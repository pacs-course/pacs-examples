#include "newton.hpp"

int
main(int argc, char **argv)
{
  auto fun = [](const double &x) -> double {
    return std::pow(x, 3) + 5 * x + 3;
  };
  auto dfun = [](const double &x) -> double { return 3 * std::pow(x, 2) + 5; };

  NewtonSolver solver(fun, dfun);
  solver.solve(0.0);

  std::cout << "x    = " << solver.get_result() << std::endl;
  std::cout << "r    = " << solver.get_residual() << std::endl;
  std::cout << "iter = " << solver.get_iter() << std::endl;

  return 0;
}
