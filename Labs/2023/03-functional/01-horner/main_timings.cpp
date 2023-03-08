/// @brief We compare the evaluation of a polynomial with the standard
/// rule against Horner's rule.

#include "horner.hpp"

#include <cmath>
#include <iostream>
#include <vector>
#include <chrono>
#include <functional>

auto timeit(const std::function<void()> &f) {
  using namespace std::chrono;
  const auto t0 = high_resolution_clock::now();
  f();
  const auto t1 = high_resolution_clock::now();
  return duration_cast<microseconds>(t1 - t0).count();
}

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

  const double x_0      = 0.00;
  const double x_f      = 1.00;
  const double h        = 0.5e-6;
  unsigned int n_points = static_cast<unsigned int>((x_f - x_0) / h);

  std::vector<double> points(n_points + 1);
  points[0] = x_0;
  for (unsigned int i = 1; i <= n_points; ++i)
    points[i] = points[i - 1] + h;

  std::cout << "Computing " << n_points << " evaluations of polynomial"
            << " with standard formula" << std::endl;
  std::cout << timeit([&](){evaluate_poly(points, coeff, &eval);}) << " [µs]" << std::endl;

  std::cout << "Computing " << n_points << " evaluations of polynomial with"
            << " Horner's rule" << std::endl;
  std::cout << timeit([&](){evaluate_poly(points, coeff, &eval_horner);}) << " [µs]" << std::endl;
}
