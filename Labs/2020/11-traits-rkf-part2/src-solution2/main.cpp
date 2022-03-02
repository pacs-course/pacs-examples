#include "RKF.hpp"

#include <cmath>
#include <fstream>
#include <iostream>

int
main(int argc, char **argv)
{
  using namespace std;

  {
    auto fun = [](double const &t, double const &y) {
      return -10 * y;
    };
    auto exact = [](double const &t) { return std::exp(-10. * t); };

    double t0           = 0;
    double y0           = 1;
    double T            = 100;
    double h_init       = 1e-5;
    double errorDesired = 1.e-4;
    double maxSteps     = 1e6;

    RKF<RKFScheme::FehlbergRK12_t, RKFKind::SCALAR> solver{
      RKFScheme::FRK12, fun};

    auto solution = solver(t0, T, y0, h_init, errorDesired, maxSteps);

    double max_error = 0.;
    int    count     = 0;

    for (auto i : solution.y)
      max_error =
        std::max(max_error,
                 std::abs(exact(solution.time[count++]) - i));

    std::cout << std::boolalpha;
    std::cout << "Max error " << max_error << " Desired max error "
              << errorDesired << " Failed:" << solution.failed
              << " Estimated Error " << solution.estimatedError;
    std::cout << std::endl;

    ofstream file("results_exp.dat");
    file << solution;
  }
  {
    // Van Der Pol with mu=1
    auto fun = [](double const &  t,
                  Eigen::VectorXd y) -> Eigen::VectorXd {
      constexpr double mu = 1.;
      Eigen::VectorXd  out(2);
      out(0) = y(1);
      out(1) = -y(0) + mu * (1 - y(0) * y(0)) * y(1);
      return out;
    };

    RKF<RKFScheme::DormandPrince_t, RKFKind::VECTOR> solver{
      RKFScheme::DP, fun};

    double t0 = 0;
    double T  = 40.;

    Eigen::VectorXd y0(2);
    y0[0] = 1.;
    y0[1] = 1.;

    double h_init       = 0.2;
    double errorDesired = 1.e-4;
    int    maxSteps     = 2000;

    auto solution = solver(t0, T, y0, h_init, errorDesired, maxSteps);

    std::cout << " Desired max error " << errorDesired
              << " Failed:" << solution.failed << " Estimated Error "
              << solution.estimatedError;
    std::cout << std::endl;

    ofstream file("results_VDP.dat");
    file << solution;
  }

  return 0;
}
