/*
 * LineSearch_traits.hpp
 *
 *  Created on: Dec 27, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_LINESEARCH_LINESEARCH_TRAITS_HPP_
#define EXAMPLES_SRC_LINESEARCH_LINESEARCH_TRAITS_HPP_
#include "Eigen/Core"
#include <functional>
namespace apsc
{
struct LineSearch_traits
{
  using Scalar = double;
  ;
  using Vector = Eigen::Matrix<Scalar, Eigen::Dynamic, 1>;
  using Matrix = Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic>;
  using CostFunction = std::function<Scalar(Vector const &)>;
  using Gradient = std::function<Vector(Vector const &)>;
};

} // namespace apsc

#endif /* EXAMPLES_SRC_LINESEARCH_LINESEARCH_TRAITS_HPP_ */
