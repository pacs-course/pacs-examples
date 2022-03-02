#include "Jacobian.hpp"
#include "JacobianFactory.hpp"
#include "Newton.hpp"
#include "NewtonTraits.hpp"

#include <iomanip>
#include <iostream>

int
main(int argc, char **argv)
{
  // Scalar problem.
  {
    constexpr auto ProbType = ProblemType::Scalar;

    using VariableType = NewtonTraits<ProbType>::VariableType;

    auto f = [](const VariableType &x) -> VariableType {
      return (x - 1) * (x - 1);
    };

    const double h = 0.1;

    auto jac_discrete = make_jacobian<ProbType, JacobianType::Discrete>(f, h);

    Newton<ProbType> quasi_newton(f, std::move(jac_discrete));

    VariableType x0 = 0;

    {
      std::cout << "*** Scalar problem, discrete jacobian ***" << std::endl;

      auto [solution, norm_res, norm_incr, iter, converged, stagnated] =
        quasi_newton.solve(x0);

      std::cout << std::boolalpha << "* Solution has converged: " << converged
                << std::endl
                << "* Solution: " << solution << std::endl
                << "* Last iteration: " << iter << std::endl
                << "* Last residual: " << norm_res << std::endl
                << "* Last increment: " << norm_incr << std::endl
                << "* Has stagnated: " << stagnated << std::endl;
    }
  }

  // Vector problem.
  {
    constexpr auto ProbType = ProblemType::Vector;

    using VariableType       = NewtonTraits<ProbType>::VariableType;
    using JacobianMatrixType = NewtonTraits<ProbType>::JacobianMatrixType;

    auto system = [](const VariableType &x) -> VariableType {
      VariableType y(2);

      y(0) = (x[0] - 1) * (x[0] - 1) + 0.1 * (x[1] - 5) * (x[1] - 5);
      y(1) = 1.5 - x[0] - 0.1 * x[1];

      return y;
    };

    NewtonOptions options;
    options.tol_res = 1e-8;

    VariableType x0(2);
    x0 << 0, 0;

    {
      std::cout << std::endl
                << std::endl
                << "*** Vector problem, full jacobian ***" << std::endl;

      auto jacobian_fun = [](const VariableType &x) -> JacobianMatrixType {
        JacobianMatrixType J(2, 2);

        J(0, 0) = 2 * (x[0] - 1);
        J(0, 1) = 0.2 * (x[1] - 5);
        J(1, 0) = -1;
        J(1, 1) = -0.1;

        return J;
      };

      auto jac_full = make_jacobian<ProbType, JacobianType::Full>(jacobian_fun);

      Newton<ProbType> newton(system, std::move(jac_full));
      auto [solution, norm_res, norm_incr, iter, converged, stagnated] =
        newton.solve(x0);

      std::cout << std::boolalpha << "* Solution has converged: " << converged
                << std::endl
                << "* Solution: "
                << solution.format(Eigen::IOFormat(4, 0, ", ", ", "))
                << std::endl
                << "* Last iteration: " << iter << std::endl
                << "* Last residual: " << norm_res << std::endl
                << "* Last increment: " << norm_incr << std::endl
                << "* Has stagnated: " << stagnated << std::endl;
    }

    {
      std::cout << std::endl
                << std::endl
                << "*** Vector problem, discrete jacobian ***" << std::endl;

      const double h = 0.1;

      auto jac_discrete =
        make_jacobian<ProbType, JacobianType::Discrete>(system, h);

      Newton<ProbType> quasi_newton(system, std::move(jac_discrete));
      auto [solution, norm_res, norm_incr, iter, converged, stagnated] =
        quasi_newton.solve(x0);

      std::cout << std::boolalpha << "* Solution has converged: " << converged
                << std::endl
                << "* Solution: "
                << solution.format(Eigen::IOFormat(4, 0, ", ", ", "))
                << std::endl
                << "* Last iteration: " << iter << std::endl
                << "* Last residual: " << norm_res << std::endl
                << "* Last increment: " << norm_incr << std::endl
                << "* Has stagnated: " << stagnated << std::endl;
    }
  }

  return 0;
}
