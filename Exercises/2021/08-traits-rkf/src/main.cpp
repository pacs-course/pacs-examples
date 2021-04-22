#include "RKF.hpp"

#include <cmath>
#include <fstream>
#include <iostream>

int
main(int argc, char **argv)
{
  // Model problem.
  {
    const auto f = [](const double &t, const double &y) { return -10 * y; };
    const auto sol_exact = [](const double &t) { return std::exp(-10 * t); };

    const double t0          = 0;
    const double tf          = 10;
    const double y0          = 1;
    const double h0          = 0.2;
    const double tolerance   = 1e-4;
    const double n_max_steps = 10000;

    RKF<RKFScheme::RK23_t, RKFType::Scalar> solver(/* ??? */, f);

    const auto solution = solver(t0, tf, y0, h0, tolerance, n_max_steps);

    // Compute error.
    double       max_error = 0;
    unsigned int count     = 0;

    for (const auto val : solution.y)
      max_error =
        std::max(max_error, std::abs(val - sol_exact(solution.time[count++])));

    std::cout << std::boolalpha;
    std::cout << "*** Model problem ***" << std::endl
              << "  l_inf error: " << max_error << std::endl
              << "  Tolerance: " << tolerance << std::endl
              << "  Failed: " << solution.failed << std::endl
              << "  Error estimate: " << solution.error_estimate << std::endl;
    std::cout << std::endl;

    std::ofstream file("results_exp.out");
    file << solution;
  }


  // Van der Pol oscillator problem with mu = 1.
  {
    const auto f = [](const double &t, const Eigen::VectorXd &y) {
      const double mu = 1;

      Eigen::VectorXd out(2);
      out(0) = y(1);
      out(1) = -y(0) + mu * (1 - y(0) * y(0)) * y(1);

      return out;
    };

    RKF<RKFScheme::RK45_t, RKFType::Vector> solver(/* ??? */, f);

    const double t0 = 0;
    const double tf = 40;

    Eigen::VectorXd y0(2);
    y0[0] = 1;
    y0[1] = 1;

    const double       h0          = 0.2;
    const double       tolerance   = 1e-4;
    const unsigned int n_max_steps = 5000;

    const auto solution = solver(t0, tf, y0, h0, tolerance, n_max_steps);

    std::cout << std::boolalpha;
    std::cout << "*** Van der Pol oscillator ***" << std::endl
              << "  Tolerance: " << tolerance << std::endl
              << "  Failed: " << solution.failed << std::endl
              << "  Error estimate: " << solution.error_estimate << std::endl;
    std::cout << std::endl;

    std::ofstream file("results_VdP.out");
    file << solution;
  }

  return 0;
}
