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

    RKFOptions options{0., 100., 1e-5, 1e-4, int(1e6)};
    RKF<RKFScheme::FehlbergRK12, RKFKind::SCALAR> solver{fun,
                                                         options};

    double y0{1};

    auto solution = solver(y0);

    double max_error = 0.;
    int    count     = 0;

    for (auto i : solution.y)
      max_error =
        std::max(max_error,
                 std::abs(exact(solution.time[count++]) - i));

    std::cout << std::boolalpha;
    std::cout << "Max error " << max_error << " Desired max error "
              << options.tol << " Failed:" << solution.failed
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

    RKFOptions options{0., 40., 0.2, 1e-4, 2000};
    RKF<RKFScheme::DormandPrince, RKFKind::VECTOR> solver{fun,
                                                          options};

    Eigen::VectorXd y0(2);
    y0[0] = 1.;
    y0[1] = 1.;

    auto solution = solver(y0);

    std::cout << " Desired max error " << options.tol
              << " Failed:" << solution.failed << " Estimated Error "
              << solution.estimatedError;
    std::cout << std::endl;

    ofstream file("results_VDP.dat");
    file << solution;
  }

  return 0;
}
