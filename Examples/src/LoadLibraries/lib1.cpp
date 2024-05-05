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
 * @fn double norm2(const std::vector<double>&)
 * @brief 2-norm of a vector
 *
 * @param x the vector
 * @return the 2-norm
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
 * @fn double norminf(const std::vector<double>&)
 * @brief Infinity (maximum) norm of a vector
 *
 * @param x the vector
 * @return the 2-norm
 */

double
norminf(std::vector<double> const &x)
{
  return *std::max_element(x.begin(), x.end());
}
} // namespace
__attribute__((constructor)) void
load()
{
  using namespace loadlibraries;
  funFactory.add("norm2", FunType{norm2});
  funFactory.add("norminf", FunType{norminf});
}
