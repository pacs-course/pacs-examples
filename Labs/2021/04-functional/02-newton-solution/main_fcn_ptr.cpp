#include "newton.hpp"

double
fun(const double &x)
{
  return std::pow(x, 3) + 5 * x + 3;
};

double
prime(const double &x)
{
  return 3 * std::pow(x, 2) + 5;
};

int
main(int argc, char **argv)
{
  newton_solver solver(fun, prime);
  solver.solve(0.0);

  std::cout << "x =    " << solver.get_result() << std::endl;
  std::cout << "r =    " << solver.get_residual() << std::endl;
  std::cout << "iter = " << solver.get_iter() << std::endl;

  return 0;
}
