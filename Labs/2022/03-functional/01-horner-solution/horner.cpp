#include "horner.hpp"

#include <algorithm>
#include <cmath>

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
 */
double
pow_integer(const double &x, const unsigned int &n)
{
  double result{x};

  for (unsigned int i = 2; i != n + 1; ++i)
    result *= x;

  return result;
}

double
eval(const std::vector<double> &a, const double &x)
{
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
#endif

  return result;
}
