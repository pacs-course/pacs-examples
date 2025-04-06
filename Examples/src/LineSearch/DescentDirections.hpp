/*
 * DescentDirections.hpp
 *
 *  Created on: Dec 28, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_LINESEARCH_DESCENTDIRECTIONS_HPP_
#define EXAMPLES_SRC_LINESEARCH_DESCENTDIRECTIONS_HPP_
#include "DescentDirectionBase.hpp"
#include "Eigen/Dense"
#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <numeric>
#include <vector>
namespace apsc
{
/*!
 * Implements the gradient search
 */
class GradientDirection : public DescentDirectionBase
{
public:
  /*!
   *  Returns - gradient as descent direction
   * @param values The current values.
   * @return The descent direction.
   */
  apsc::LineSearch_traits::Vector
  operator()(apsc::OptimizationCurrentValues const &values) const override
  {
    return -values.currentGradient;
  }
  /*!
   * @brief The class is clonable
   *
   * @return A clone of myself wrapped into a unique pointer
   */
  virtual std::unique_ptr<DescentDirectionBase>
  clone() const override
  {
    return std::make_unique<GradientDirection>(*this);
  }
};
/*!
 * Implements the Newton search
 */
class NewtonDirection : public DescentDirectionBase
{
public:
  /*!
   *  Returns - gradient as descent direction
   * @param values The current values.
   * @return The descent direction.
   */
  apsc::LineSearch_traits::Vector
  operator()(apsc::OptimizationCurrentValues const &values) const override;

  /*!
   * @brief The class is clonable
   *
   * @return A clone of myself wrapped into a unique pointer
   */
  virtual std::unique_ptr<DescentDirectionBase>
  clone() const override
  {
    return std::make_unique<NewtonDirection>(*this);
  }

private:
  static constexpr double eps = 100. * std::numeric_limits<double>::epsilon();
};
/*!
 *  Implements the classic BFGS quasi-Newton algorithm.
 */
class BFGSDirection : public DescentDirectionBase
{
public:
  apsc::LineSearch_traits::Vector
  operator()(apsc::OptimizationCurrentValues const &values) const override;
  /*!
   * You need to reset if you run another problem or you start from a different
   * initial point.
   * @note This is done inside LineSearchSolver
   */
  void reset() override;
  /*!
   * @brief The class is clonable
   *
   * @return A clone of myself wrapped into a unique pointer
   */
  virtual std::unique_ptr<DescentDirectionBase>
  clone() const override
  {
    return std::make_unique<BFGSDirection>(*this);
  }

private:
  mutable apsc::OptimizationCurrentValues previousValues;
  mutable Eigen::MatrixXd                 H;
  mutable bool                            firstTime{true};
  double const smallNumber = std::sqrt(std::numeric_limits<double>::epsilon());
};

/*!
 * Implements BFGS with the direct computation of the approximate inverse of
 * the Hessian.
 */
class BFGSIDirection : public DescentDirectionBase
{
public:
  apsc::LineSearch_traits::Vector
       operator()(apsc::OptimizationCurrentValues const &values) const override;
  void reset() override;

private:
  mutable apsc::OptimizationCurrentValues previousValues;
  mutable Eigen::MatrixXd                 H;
  mutable bool                            firstTime{true};
  double const smallNumber = std::sqrt(std::numeric_limits<double>::epsilon());

  /*!
   * @brief The class is clonable
   *
   * @return A clone of myself wrapped into a unique pointer
   */
  virtual std::unique_ptr<DescentDirectionBase>
  clone() const override
  {
    return std::make_unique<BFGSIDirection>(*this);
  }
};
/*!
 * Bazrzilain-Borwein
 */
class BBDirection : public DescentDirectionBase
{
public:
  apsc::LineSearch_traits::Vector
  operator()(apsc::OptimizationCurrentValues const &values) const override;
  void
  reset() override
  {
    firstTime = true;
  };
  /*!
   * @brief The class is clonable
   *
   * @return A clone of myself wrapped into a unique pointer
   */
  virtual std::unique_ptr<DescentDirectionBase>
  clone() const override
  {
    return std::make_unique<BBDirection>(*this);
  }

private:
  mutable apsc::OptimizationCurrentValues previousValues;
  mutable bool                            firstTime{true};
  double const smallNumber = std::sqrt(std::numeric_limits<double>::epsilon());
};
/*!
 * Non-linear CG with Polak-Ribiere formula
 */
class CGDirection : public DescentDirectionBase
{
public:
  apsc::LineSearch_traits::Vector
  operator()(apsc::OptimizationCurrentValues const &values) const override;
  void
  reset() override
  {
    firstTime = true;
  };

  /*!
   * @brief The class is clonable
   *
   * @return A clone of myself wrapped into a unique pointer
   */
  virtual std::unique_ptr<DescentDirectionBase>
  clone() const override
  {
    return std::make_unique<CGDirection>(*this);
  }

private:
  mutable apsc::OptimizationCurrentValues previousValues;
  mutable bool                            firstTime{true};
  //! I need to keep track of previous descent direction
  mutable apsc::LineSearch_traits::Vector prevDk;
};
} // namespace apsc

#endif /* EXAMPLES_SRC_LINESEARCH_DESCENTDIRECTIONS_HPP_ */
