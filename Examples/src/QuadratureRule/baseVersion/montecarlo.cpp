#include "montecarlo.hpp"
#include <cmath>
#include <iostream>
namespace apsc::NumericalIntegration
{
MonteCarlo::MonteCarlo()
  : max_iter(max_iter_def), error_level(error_level_def), M_lastError(0),
    M_cumulatedError(0), M_iterationsExceeded(false)
{}

std::unique_ptr<QuadratureRuleBase>
MonteCarlo::clone() const
{
  return std::make_unique<MonteCarlo>(*this);
}

void
MonteCarlo::setError(double e)
{
  error_level = e;
}
void
MonteCarlo::setMiter(unsigned int m)
{
  max_iter = m;
}

double
MonteCarlo::apply(FunPoint const &f, double const &a, double const &b) const
{
  using namespace std;
  static constexpr unsigned int min_iter = 16;

  uniform_real_distribution<> dis{a, b};
  unsigned int                n{0};
  double                      mean{0};
  double                      S{0};
  double                      error{2 * error_level}; // just to enter the loop
  while((n < max_iter && error > error_level) || n < min_iter)
    {
      ++n;
      double x = dis(rengine);
      double y = f(x);
      double delta = y - mean;
      // Adjourn mean
      mean += delta / n;
      // Adjourn s.dev.
      S += delta * (y - mean);
      // Adjourn error estimate
      error = (b - a) * sqrt(S) / n;
    }
  M_lastError = error;
  M_cumulatedError += error;
  M_iterationsExceeded = (n >= max_iter);
  if(M_iterationsExceeded)
    std::clog << "Monte Carlo N. iterations exceeded"
              << " Error " << error << " Target error " << error_level
              << std::endl;
  return (b - a) * mean;
}

void
MonteCarlo::resetError()
{
  M_cumulatedError = 0;
  M_iterationsExceeded = false;
}

} // namespace apsc::NumericalIntegration
