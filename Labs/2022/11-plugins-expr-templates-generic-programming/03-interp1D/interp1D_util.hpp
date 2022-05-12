#ifndef EXAMPLES_SRC_INTERP1D_INTERP1D_UTIL_HPP_
#define EXAMPLES_SRC_INTERP1D_INTERP1D_UTIL_HPP_

#include "interp1D.hpp"

#include <numeric>
#include <vector>

//
/*! Implementation for vector<T> where T behaves like an array with
 * 2 components (key value).
 *
 * @tparam CompOper Comparison operator
 * @tparam T the type stored in the array. it should behave as an array of 2
 * components. The first is the key, the second is the value
 * @param v a vector
 * @param keyVal the point to be interpolated
 * @param comp Comparison operator, defaults to less<T>
 * @return
 */
template <class T, class CompOper = std::less<double>>
double
interp1D(std::vector<T> const &v,
         double const &        keyVal,
         CompOper const &      comp = std::less<double>{})
{}


/*!
 * Implementation when interpolation nodes and values are stored in
 * two different vectors
 *
 * @tparam T  The type of values to be interpolated
 * @tparam CompOper The comparison operator among keys (default less<double>)
 * @param keys The vector containing the keys (interpolation nodes)
 * @param values The vector containing the values
 * @param keyVal The value of the key where I have to interpolate
 * @param comp The comparison operator (defaulted to less<double>{}).
 * @return
 */
template <class T, class CompOper = std::less<double>>
T
interp1D(std::vector<double> const &keys,
         std::vector<T> const &     values,
         double const &             keyVal,
         CompOper const &           comp = std::less<double>{})
{}

#endif /* EXAMPLES_SRC_INTERP1D_INTERP1D_UTIL_HPP_ */
