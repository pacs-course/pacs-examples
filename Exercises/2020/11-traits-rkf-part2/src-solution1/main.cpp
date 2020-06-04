#include "RKF.hpp"

#include <cmath>
#include <fstream>
#include <iostream>

int
main(int argc, char **argv)
{
  using namespace std;

  // Input coefficients are related to the region of Lombardy
  // and are taken from https://arxiv.org/pdf/2003.14391.pdf.
  constexpr unsigned int n = 1.006e6;

  auto fun = [&n](double const &t, Eigen::VectorXd const &y) {
    constexpr double beta  = 0.189301;
    constexpr double gamma = 0.0307642;

    Eigen::VectorXd out(3);
    out(0) = -beta * y(0) * y(1) / n;
    out(1) = beta * y(0) * y(1) / n - gamma * y(1);
    out(2) = gamma * y(1);

    return out;
  };

  Eigen::VectorXd y0(3);
  y0(0) = n - 100;
  y0(1) = n - y0(0);
  y0(2) = 0;

  double       t0        = 0;
  double       tf        = 180;
  double       h_init    = 1;
  double       tolerance = 1e-4;
  unsigned int max_steps = 1e3;

  RKF<RKFScheme::RK45_t, RKFKind::VECTOR> solver{RKFScheme::RK45,
                                                 fun};

  auto solution = solver(t0, tf, y0, h_init, tolerance, max_steps);

  ofstream file("results_SIR.dat");
  file << solution;

  return 0;
}
