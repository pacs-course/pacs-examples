/*
 * LineSearchSolver.hpp
 *
 *  Created on: Dec 28, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_LINESEARCH_LINESEARCHSOLVER_HPP_
#define EXAMPLES_SRC_LINESEARCH_LINESEARCHSOLVER_HPP_
#include "DescentDirectionBase.hpp"
#include "LineSearch.hpp"
#include <memory>
#include <tuple>
namespace apsc
{
/*!
 * The class that implements a line search technique for unconstrained
 * minimization of a function.
 */
class LinearSearchSolver
{
public:
  LinearSearchSolver() = default;
  /*!
   * You may pass all needed parameters in the constructors
   *
   * @param data The structure containing the cost function and the gradient
   * function
   * @param ddptr a unique pointer to the concrete callable object that computes
   * the search direction
   * @param opt The options for the line search iterations.
   * @param lsOpt The options for the backtracking function.
   * @note The object containing the definition of the desscent direction is
   * MOVED into the class. So you have to be sure that the argument is movable.
   * I have not implemented the clone method for DescentDirectionBase, so this
   * class is NOT copy constructible, not copy-assigneable.
   */
  LinearSearchSolver(
    apsc::OptimizationData const &                data,
    std::unique_ptr<apsc::DescentDirectionBase> &&ddptr,
    apsc::OptimizationOptions opt = apsc::OptimizationOptions(),
    apsc::LineSearchOptions   lsOpt = apsc::LineSearchOptions())
    : optimizationData(data), options(opt), lineSearchOptions(lsOpt),
      descentDirectionFinderPtr(std::move(ddptr))
  {}

  /*!
   * You need to set the intitial point for the search.
   * @param initialPoint The initial point fo the search.
   */
  void setInitialPoint(apsc::LineSearch_traits::Vector initialPoint);

  std::tuple<apsc::OptimizationCurrentValues, std::size_t, int> solve();

  /*!
   * You may change the rule to compute the descent direction.
   * @note It is moved into the class.
   * @param descentDirectionFinderPtr The new object that computed the descent
   * direction
   */
  void
  setDescentDirectionFinderPtr(
    std::unique_ptr<apsc::DescentDirectionBase> &&descentDirectionFinderPtr)
  {
    this->descentDirectionFinderPtr = std::move(descentDirectionFinderPtr);
  }

  //! The structure holding the cost function and the gradient
  apsc::OptimizationData optimizationData;
  //! The structure containing the options for the line search algorithm
  apsc::OptimizationOptions options;
  //! The structure containing the options for the backtracking.
  apsc::LineSearchOptions lineSearchOptions;

private:
  /*!
   * Implements a simple backtracking
   * @param searchDirection The search directions
   * @return The new point, the new cost value and a status. status=1 ->
   * direction is not a descent direction. status=2 -> number of attempts
   * exceeded
   */
  std::tuple<apsc::LineSearch_traits::Vector, apsc::LineSearch_traits::Scalar,
             int>
  backtrack(apsc::LineSearch_traits::Vector  &searchDirection) const;
  apsc::OptimizationCurrentValues             currentValues;
  std::unique_ptr<apsc::DescentDirectionBase> descentDirectionFinderPtr;
  std::tuple<apsc::LineSearch_traits::Vector, bool>
  project(apsc::LineSearch_traits::Vector const &newPoint) const;
  apsc::LineSearch_traits::Vector
  projectGrad(apsc::LineSearch_traits::Vector const &newPoint,
              apsc::LineSearch_traits::Vector const & gradient) const;
 void projectDirection(apsc::LineSearch_traits::Vector & searchDirection) const;
};

} // namespace apsc

#endif /* EXAMPLES_SRC_LINESEARCH_LINESEARCHSOLVER_HPP_ */
