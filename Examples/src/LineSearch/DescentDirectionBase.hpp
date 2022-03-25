/*
 * LineaSearchSolvers.hpp
 *
 *  Created on: Dec 27, 2020
 *      Author: forma
 */
#include "LineSearch.hpp"
#ifndef EXAMPLES_SRC_LINESEARCH_DESCENTDIRECTIONBASE_HPP_
#define EXAMPLES_SRC_LINESEARCH_DESCENTDIRECTIONBASE_HPP_
#include <memory>
namespace apsc
{
/*!
 * The base class for the callable objects that compute the descent direction
 */
class DescentDirectionBase
{
public:
  /*!
   * Given the current values (point, gradient etc) as defined in
   * apsc::OptimizationCurrentValues returns a descent direction.
   * @param values The current values.
   * @return The descent direction
   * @note It is not declared const since in some derived class it may modify
   * the state of the object.
   */
  virtual apsc::LineSearch_traits::Vector
  operator()(apsc::OptimizationCurrentValues const &values) = 0;
  /*!
   * This function by default it does nothing, but it may be used to reset data
   * if needed
   */
  virtual void
  reset(){};
  virtual ~DescentDirectionBase() = default;
  /*!
   * @brief The class is clonable
   *
   * @return A clone of myself wrapped into a unique pointer
   */
  virtual
  std::unique_ptr<DescentDirectionBase>
  clone() const {return nullptr;}
};

} // namespace apsc

#endif /* EXAMPLES_SRC_LINESEARCH_LINEASEARCHSOLVERS_HPP_ */
