#ifndef __INTERPOLATION_HPP__
#define __INTERPOLATION_HPP__
#include "interpolationPolicies.hpp"

//! @file interpolation.hpp
/*!
  @brief some utilities for 1D interpolation
  @details It explains the use of functors as policies.
 */
namespace apsc
{


  //! Computes interpolation at several points.
  /*
    @par xcoord  Coordinates where we want to compute the interpolation.
    @par interpolator The policy implementing the interpolation
    @return The computed interpolated values
    @note default value for function template parameters is a C++11 extension.
  */
  template
  <class T, class Interpolator>
  std::vector<T> interp1D(const std::vector<T>& xcoord, Interpolator interpolator)
  {
    std::vector<T> v;
    v.reserve(xcoord.size());
    for (auto const & i : xcoord) v.emplace_back(interpolator(i));
    return v;
  }
}
#endif
