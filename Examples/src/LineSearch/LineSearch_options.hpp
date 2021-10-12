/*
 * LineSearch_options.hpp
 *
 *  Created on: Dec 27, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_LINESEARCH_LINESEARCH_OPTIONS_HPP_
#define EXAMPLES_SRC_LINESEARCH_LINESEARCH_OPTIONS_HPP_
#include "LineSearch_traits.hpp"
namespace apsc
{
struct LineSearchOptions
{
  using Scalar = apsc::LineSearch_traits::Scalar;
  Scalar sufficientDecreaseCoefficient =
    1.e-2; //!< The coefficient for sufficient decrease
  Scalar stepSizeDecrementFactor = 0.5; //!< How much to decrement alpha
  Scalar secondWolfConditionFactor =
    0.9;                          //!< Second Wolfe conditin factor (not used)
  Scalar       initialStep = 1.0; //!< the initial apha value.
  unsigned int maxIter =
    40; //!< max number of iterations in the backtracking algorithm
};

} // namespace apsc

#endif /* EXAMPLES_SRC_LINESEARCH_LINESEARCH_OPTIONS_HPP_ */
