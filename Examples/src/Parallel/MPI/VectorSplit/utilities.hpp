/*
 * utilities.hpp
 *
 *  Created on: Aug 12, 2022
 *      Author: forma
 */

#ifndef AMSC_EXAMPLES_EXAMPLES_SRC_PARALLEL_MPI_VECTORSPLIT_UTILITIES_HPP_
#define AMSC_EXAMPLES_EXAMPLES_SRC_PARALLEL_MPI_VECTORSPLIT_UTILITIES_HPP_
#include <vector>
#include <array>
#include <random>
#include <algorithm>
namespace utilities
{
/*!
 * Fills a vector with randome numbers normally distrobuted with N(0,5)
 * @param v The vector
 */
inline void fill_random(std::vector<double> & v)
{
  std::random_device rd{};
  std::mt19937 gen{rd()};
  std::normal_distribution<> d{0.,5.};
  for (auto & x:v)
    x=d(gen);
}

/*!
 * Find the mean of elements in a vector
 * @param v the vector
 * @return The mean value
 */
inline double mean(std::vector<double> const & v)
{
  double res=0;
  for (auto x:v)
    res+=x;
  return res/v.size();
}

/*!
 * Find min and max elements of the vector
 * @param v The vector
 * @return a pair with the min and max values.
 */
inline std::array<double,2> minmax(std::vector<double> const & v)
    {
  auto resit = std::minmax_element(v.begin(), v.end());
  return {*resit.first,*resit.second};
    }
} // end namespace utilities


#endif /* AMSC_EXAMPLES_EXAMPLES_SRC_PARALLEL_MPI_VECTORSPLIT_UTILITIES_HPP_ */
