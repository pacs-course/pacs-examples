#include "JacobianFactory.hpp"
#include "Newton.hpp"

#include <iomanip>
#include <iostream>

int
main(int argc, char **argv)
{
  using ArgumentType       = NewtonTraits::ArgumentType;
  using ReturnType         = NewtonTraits::ReturnType;
  using JacobianMatrixType = NewtonTraits::JacobianMatrixType;

  auto system = [](const ArgumentType &x) -> ReturnType {
    ReturnType y(2);

    y(0) = (1 - x(0)) * (1 - x(0)) + 0.1 * (5 - x(1)) * (5 - x(1));
    y(1) = 1.5 - x(0) - 0.1 * x(1);

    return y;
  };

  auto jacobianFun = [](const ArgumentType &x) -> JacobianMatrixType {
    JacobianMatrixType J(2, 2);

    J(0, 0) = -2 * (1 - x(0));
    J(0, 1) = -0.2 * (5 - x(1));
    J(1, 0) = -1;
    J(1, 1) = -0.1;

    return J;
  };

  FullJacobian J(jacobianFun);

  NewtonOptions options;
  options.minRes = 1e-8;

  NewtonVerbose solver(system, J, options);

  ArgumentType x0(2);
  x0 << 0, 0;

  {
    std::cout << "*** Newton solver (full jacobian) ***" << std::endl;

    auto [solution, resNorm, stepLength, iter, converged, stagnated] =
      solver.solve(x0);

    std::cout << std::boolalpha
              << "* Solution has converged: " << converged
              << std::endl
              << "* Last iteration: " << iter << std::endl
              << "* Last residual: " << resNorm << std::endl
              << "* Has stagnated: " << stagnated << std::endl;
  }

  solver.setJacobianPtr(
    make_Jacobian<JacobianType::Discrete>(nullptr, 10));

  {
    std::cout << std::endl
              << std::endl
              << "*** Newton solver (discrete jacobian) ***"
              << std::endl;

    auto [solution, resNorm, stepLength, iter, converged, stagnated] =
      solver.solve(x0);

    std::cout << std::boolalpha
              << "* Solution has converged: " << converged
              << std::endl
              << "* Last iteration: " << iter << std::endl
              << "* Last residual: " << resNorm << std::endl
              << "* Has stagnated: " << stagnated << std::endl;
  }

  return 0;
}
