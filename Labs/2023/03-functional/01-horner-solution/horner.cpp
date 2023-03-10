#include "horner.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

// Parallel policies are only available in GCC since version 9.
#ifdef PARALLELEXEC
#  if defined(__GNUC__) && (__GNUC__ >= 9)
#    include <execution>
#  else
#    undef PARALLELEXEC
#  endif
#endif

/**
 * Since std::pow() is very expensive, a specialization for
 * integers is implemented.
 * https://en.wikipedia.org/wiki/Exponentiation_by_squaring
 */
double pow_integer(double base, unsigned int exp) {
  double res = 1.0;
  while (exp > 0) {
    if (exp & 1)
      res *= base;
    base *= base;
    exp >>= 1;
  }
  return res;
}

double
eval(const std::vector<double> &a, const double &x)
{
  if (a.empty())
    std::cerr << "The vector of coefficients 'a' cannot be empty!" << std::endl;

  double result{a[0]};

  for (std::vector<double>::size_type k = 1; k < a.size(); ++k)
    {
      result += a[k] * pow_integer(x, k);

      // std::pow is VERY expensive.
      // If you want to test with the standard pow comment the
      // previous statement and uncomment the next one.
      // result += a[k] * std::pow(x, k);
    }

  return result;
}

double
eval_horner(const std::vector<double> &a, const double &x)
{
  double result{a.back()};

  for (auto i = a.crbegin() + 1; i != a.crend(); ++i)
    result = result * x + (*i);

  return result;
}

/// Evaluates polynomial at a set of points.
std::vector<double>
evaluate_poly(const std::vector<double> &points,
              const std::vector<double> &a,
              eval_method                method)
{
  std::vector<double> result(points.size());

  auto compute = [&a, &method](const double &x) { return method(a, x); };

#ifdef PARALLELEXEC
#  warning "Using parallel implementation of std::transform"
  std::transform(
    std::execution::par, points.begin(), points.end(), result.begin(), compute);
#else
#  warning "Using sequential implementation of std::transform"
  std::transform(points.begin(), points.end(), result.begin(), compute);
  // Or, equivalently:
  // std::transform(std::execution::seq, points.begin(), points.end(), result.begin(), compute);
#endif

  return result;
}
