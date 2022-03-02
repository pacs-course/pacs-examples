//
//! @brief  A test of the class Chrono and of polynomial evaluation.
//! @detail We compare the evaluation of a polygon with the standard
//! rule against Horner's rule.

#include "chrono.hpp"
#include "horner.hpp"

#include <cmath>
#include <iostream>
#include <vector>

int
main(int argc, char **argv)
{
  unsigned int degree;
  std::cout << "Polynomial degree" << std::endl;
  std::cout << "=> ";
  std::cin >> degree;

  std::vector<double> coeff(degree + 1);
  std::cout << "Coefficients are computed automatically" << std::endl;
  for (unsigned int i = 0; i <= degree; ++i)
    coeff[i] = 2 * std::sin(2.0 * i);

  /*
  std::cout << "Give me coefficients a0, a1..." << std::endl;
  for (unsigned int i = 0; i <= degree; ++i) {
    double temp;
    std::cout << "a[" << i << "]=";
    std::cin >> temp;
    std::cout << std::endl;
    coeff.push_back(temp);
  }
  */

  double       x_0      = 0.00;
  double       x_f      = 1.00;
  double       h        = 0.5e-6;
  unsigned int n_points = static_cast<unsigned int>((x_f - x_0) / h);

  std::vector<double> points(n_points + 1);

  double x = x_0;
  for (unsigned int i = 0; i <= n_points; ++i, x += h)
    points[i] = x;

  Timings::Chrono timer;

  std::cout << "Computing " << n_points << " evaluation of polynomial"
            << " with standard formula" << std::endl;
  timer.start();
  evaluate_poly(points, coeff, &eval);
  std::cout << std::endl;
  timer.stop();
  std::cout << timer << std::endl;

  std::cout << "Computing " << n_points << " evaluation of the polynomial with"
            << " Horner's rule" << std::endl;
  timer.start();
  evaluate_poly(points, coeff, &eval_horner);
  std::cout << std::endl;
  timer.stop();
  std::cout << timer << std::endl;
}
