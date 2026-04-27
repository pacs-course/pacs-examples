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
 * @brief Computes the 1-norm of a vector.
 * @param x Input vector.
 * @return The value of `sum_i |x_i|`.
 */
double
norm1(std::vector<double> const &x)
{
  auto res = 0.0;
  for(auto const &y : x)
    res += std::abs(y);
  return res;
}

/*!
 * @brief Counts the nonzero-valued entries of a vector.
 * @param x Input vector.
 * @return Number of elements not equal to `0.0`.
 *
 * Despite the name, this is not the sparsity-oriented "zero norm" usually
 * defined in numerical analysis, which counts non-zero entries.
 */
double
norm0(std::vector<double> const &x)
{
  return std::count_if(x.begin(), x.end(),
                       [](double const &y) { return y != 0.; });
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
  funFactory.add("norm1", FunType{norm1});
  funFactory.add("norm0", FunType{norm0});
}
