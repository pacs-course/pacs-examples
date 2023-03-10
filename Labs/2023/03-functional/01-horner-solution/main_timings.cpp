/// @brief  A test of the class Chrono, json and of polynomial evaluation.
/// @detail We compare the evaluation of a polynomial with the standard
/// rule against Horner's rule.

// to use this you have to have compiled and installed the pacs "Utilities"
#include "chrono.hpp"
// documentation at https://github.com/nlohmann/json
#include "json.hpp"

// header in this folder
#include "horner.hpp"

#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>


int
main(int argc, char** argv)
{
  using json = nlohmann::json;
  std::ifstream f("data.json");
  json data = json::parse(f);

  std::cout << "--- Paramenters:" << std::endl;
  const unsigned int degree = data.value("degree", 0);
  std::cout << "Polynomial degree: " << degree << std::endl;
  const double x_0 = data.value("x_0", 0.0);
  std::cout << "x_0: " << x_0 << std::endl;
  const double x_f = data.value("x_f", 1.0);
  std::cout << "x_f: " << x_f << std::endl;
  const unsigned int n_points = data.value("n_points", 1'000'000);
  std::cout << "n_points: " << n_points << std::endl;
  const double h = (x_f - x_0) / (n_points - 1);
  std::cout << "---------------------------" << std::endl;

  std::vector<double> coeff(degree + 1);
  std::cout << "Coefficients are computed automatically" << std::endl;
  for (unsigned int i = 0; i <= degree; ++i)
    coeff[i] = 2 * std::sin(2.0 * i);

  std::vector<double> points(n_points + 1);
  points[0] = x_0;
  for (unsigned int i = 1; i <= n_points; ++i)
    points[i] = points[i - 1] + h;

  Timings::Chrono timer;

  std::cout << "Computing " << n_points << " evaluations of polynomial"
    << " with standard formula" << std::endl;
  timer.start();
  evaluate_poly(points, coeff, &eval);
  std::cout << std::endl;
  timer.stop();
  std::cout << timer << std::endl;

  std::cout << "Computing " << n_points << " evaluations of polynomial with"
    << " Horner's rule" << std::endl;
  timer.start();
  evaluate_poly(points, coeff, &eval_horner);
  std::cout << std::endl;
  timer.stop();
  std::cout << timer << std::endl;
}
