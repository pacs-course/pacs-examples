/*
 * Optimization_options.hpp
 *
 *  Created on: Dec 27, 2020
 *      Author: forma
 */
#ifndef EXAMPLES_SRC_LINESEARCH_OPTIMIZATION_OPTIONS_HPP_
#define EXAMPLES_SRC_LINESEARCH_OPTIMIZATION_OPTIONS_HPP_
#include "LineSearch_traits.hpp"
#include <exception>
#include <vector>
namespace apsc
{
/*!
 * It holds all options for the line search algorithm
 */
struct OptimizationOptions
{
  using Scalar = apsc::LineSearch_traits::Scalar;
  Scalar       relTol = 1.e-5; //!< relative tolerance
  Scalar       absTol = 1.e-6; //!< absolute tolerance
  unsigned int maxIter = 500;  //!< max n. of Iteration
};

/*!
 * It holds the main data for the optimization algorithm
 */
struct OptimizationData
{
  apsc::LineSearch_traits::CostFunction costFunction; //!< The cost function.
  apsc::LineSearch_traits::Gradient
              gradient;              //!< The gradient of the cost function.
  //! The Hessian: by default an empty matrix
  apsc::LineSearch_traits::Hessian hessian=[](apsc::LineSearch_traits::Vector const &){return apsc::LineSearch_traits::Matrix{};};
  std::size_t NumberOfVariables = 0; //! The number of variables of the problem.
  bool        bounded = false;       //! We may have bound constraints
  std::vector<double> lowerBounds;   //! Uses if bounded=true
  std::vector<double> upperBounds;   //! Uses if bounded=true
};

inline void
setBounds(OptimizationData &optimizationData, std::vector<double> const &lo,
          std::vector<double> const &up)
{
  if(optimizationData.NumberOfVariables > lo.size() and
     optimizationData.NumberOfVariables > up.size())
    {
      throw std::runtime_error("Wrong bound sizes");
    }
  optimizationData.bounded = true;
  optimizationData.lowerBounds = lo;
  optimizationData.upperBounds = up;
}

/*!
 * A structure used to hold the current values.
 */
struct OptimizationCurrentValues
{
  apsc::LineSearch_traits::Scalar currentCostValue; //!< current cost.
  apsc::LineSearch_traits::Vector currentPoint;     //!< current point.
  apsc::LineSearch_traits::Vector currentGradient;  //!< current gradient.
  apsc::LineSearch_traits::Matrix currentHessian;  //!< current Hessian.
  bool        bounded = false;       //! We may have bound constraints
  std::vector<double> lowerBounds;   //! Uses if bounded=true
  std::vector<double> upperBounds;   //! Uses if bounded=true
};
} // namespace apsc

#endif /* EXAMPLES_SRC_LINESEARCH_OPTIMIZATION_OPTIONS_HPP_ */
