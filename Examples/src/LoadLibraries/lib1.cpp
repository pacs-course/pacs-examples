/*
 * lib1.cpp
 *
 *  Created on: Jan 7, 2022
 *      Author: forma
 */

#include <algorithm>

#include "libtraits.hpp"

namespace
{
/*!
 * @brief Computes the Euclidean norm of a vector.
 * @param x Input vector.
 * @return The value of `sqrt(sum_i x_i^2)`.
 */
double
norm2(std::vector<double> const &x)
{
  auto res = 0.0;
  for(auto const &y : x)
    res += y * y;
  return std::sqrt(res);
}
/*!
 * @brief Computes the infinity norm of a vector.
 * @param x Input vector.
 * @return The value of `max_i |x_i|`.
 *
 */
double
norminf(std::vector<double> const &x)
{
  return std::abs(*std::max_element(x.begin(), x.end(), [](double x, double y) {
    return std::abs(x) < std::abs(y);
  }));
}
} // namespace

/*!
 * @brief Registers the functions exported by this shared library.
 *
 * The GNU constructor attribute makes this function run automatically when the
 * shared object is loaded with `dlopen()`.
 */
__attribute__((constructor)) void
load()
{
  using namespace loadlibraries;
  funFactory.add("norm2", FunType{norm2});
  funFactory.add("norminf", FunType{norminf});
}
