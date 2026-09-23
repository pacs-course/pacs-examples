/*! \brief Solve a nonlinear system using an autodiff-computed Jacobian. */
#include "Newton_ad.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numbers>

int
main()
{
  using ArgumentType = apsc::newton_ad::NewtonTraits::AutoDiffArgumentType;
  using namespace autodiff;

  /*!
   * \brief The nonlinear system to be solved.
   * \param x The argument vector.
   * \return The residual vector.
   * \note The argument and return types are defined in the NewtonTraits_ad.hpp
   * file and are autodiff::VectorXreal, which is a vector of Real<1,double>
   * numbers. The Real<1,double> type is a number that can compute its
   * derivative using forward mode autodiff.
   */
  auto nonLinSys = [](const ArgumentType &x) {
    constexpr double pi = std::numbers::pi_v<double>;
    ArgumentType     y(3);
    y(0) = 3.0 * x(0) - cos(x(1) * x(2)) - 0.5;
    y(1) = x(0) * x(0) - 81.0 * (x(1) + 0.1) * (x(1) + 0.1) + sin(x(2)) + 1.06;
    y(2) = exp(-x(0) * x(1)) + 20.0 * x(2) + (10.0 * pi - 3.0) / 3.0;
    return y;
  };

  apsc::newton_ad::NewtonOptions options;
  options.backtrackOn =
    true; // Use backtracking line search to improve convergence
  apsc::newton_ad::NewtonVerbose solver(nonLinSys,
                                        options); // I use the verbose version

  Eigen::VectorXd x0(3);
  x0 << 0.0, 0.0, 0.0;                  // Initial guess for the solution
  const auto result = solver.solve(x0); // Solve the nonlinear system

  std::cout << std::boolalpha << std::setprecision(12)
            << "Newton solution: " << result.solution.transpose() << '\n'
            << "Converged: " << result.converged
            << " iterations: " << result.iterations
            << " residual: " << result.residualNorm << '\n';
}
