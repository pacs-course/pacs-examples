#include "horner.hpp"
#include <algorithm>
#include <cmath>
// Comment/uncomment next line if you dont want/want parallelization
#ifdef PARALLELEXEC
#include <execution>
#endif

double
eval(std::vector<double> const &a, double const &x)
{
  double sum = a[0];
  for(std::size_t k = 1; k < a.size(); ++k)
    {
      sum += a[k] * std::pow(x, k);
    }
  return sum;
}

double
horner(std::vector<double> const &a, double const &x)
{
  double u = a.back(); // last value
  for(auto i = a.crbegin() + 1; i != a.crend(); ++i)
    u = u * x + *i;
  return u;
}

/*
This version used the new ranges library. Look how nicer
it is compared to the previous version.
*/
#include <ranges>

double
horner_range(std::vector<double> const &a, double const &x)
{
  double u = a.back(); // last value
  for(auto const &i : a | std::views::reverse | std::views::drop(1))
    u = u * x + i;

  return u;
}

//! Evaluates polynomial in a set of points (parallel version if PARALLELEXEC is
//! set)
std::vector<double>
evaluatePoly(std::vector<double> const &points, std::vector<double> const &a,
             polyEval method)
{
  std::vector<double> result(points.size());
  // if you prefer a normal loop
  // for (std::size_t i=0;i<points.size();++i) result[i]=method(a,points[i]);
  // Here I use std::transform to be consistent with the parallel version
  auto compute = [&a, &method](double const &x) { return method(a, x); };
#ifdef PARALLELEXEC
#pragma message("Using parallel implementation of std::transform")
  std::transform(std::execution::par, points.begin(), points.end(),
                 result.begin(), compute);
#else
  std::transform(points.begin(), points.end(), result.begin(), compute);
#endif
  return result;
}