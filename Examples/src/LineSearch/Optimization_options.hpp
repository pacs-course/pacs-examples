/*
 * Optimization_options.hpp
 *
 *  Created on: Dec 27, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_LINESEARCH_OPTIMIZATION_OPTIONS_HPP_
#define EXAMPLES_SRC_LINESEARCH_OPTIMIZATION_OPTIONS_HPP_
#include "LineSearch_traits.hpp"
namespace apsc
{
  /*!
   * It holds all options for the line search algorithm
   */
struct OptimizationOptions
  {
    using Scalar=apsc::LineSearch_traits::Scalar;
    Scalar relTol=1.e-5; //!<relative tolerance
    Scalar absTol=1.e-6; //!<absolute tolerance
    unsigned int maxIter=500; //!<max n. of Iteration
  };

/*!
 * It holds the main data for the optimization algorithm
 */
struct OptimizationData
{
  apsc::LineSearch_traits::CostFunction costFunction; //!<The cost function.
  apsc::LineSearch_traits::Gradient gradient; //!<The gradient of the cost function.
  std::size_t NumberOfVariables=0; //! The number of variables of the problem.
};

/*!
 * A structure used to hold the current values.
 */
struct OptimizationCurrentValues
{
  apsc::LineSearch_traits::Scalar currentCostValue; //!< current cost.
  apsc::LineSearch_traits::Vector currentPoint; //!< current point.
  apsc::LineSearch_traits::Vector currentGradient;//!< current gradient.
};
}


#endif /* EXAMPLES_SRC_LINESEARCH_OPTIMIZATION_OPTIONS_HPP_ */
