#pragma once
// I aliminate a few diacnostic produced by the autodiff library, due to
// the use of deprecated features in the autodiff library.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-override"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wdeprecated"
#pragma GCC diagnostic ignored "-Wparentheses"
#include "LineSearch_traits.hpp"
#include "autodiff/forward/real/eigen.hpp"
#include "autodiff/reverse/var.hpp"
#include "autodiff/reverse/var/eigen.hpp"
#include <tuple>
namespace apsc
{
/*!
@brief A class that computes the cost, gradient and Hessian of a cost function
using automatic differentiation.

@details
The class uses the autodiff library to compute the cost, gradient and Hessian of
a given cost function. The cost function is expected to be a callable object
that takes an Eigen vector as input and returns a scalar value. The class is
meant to offer and easy interface for the LineSearch class, which requires the
cost, gradient and Hessian to be provided as callable functions.

I am using reverse mode automatic differentiation, which is more efficient for
functions with many inputs and few outputs, as is the case for the cost function
in optimization problems. However, this implementation is not optimal, since
it computes the cost, gradient and Hessian separately, which is not efficient in
reverse mode.

A more efficient implementation would compute thhe three values in a single
pass, but it would require a more complex refactoring of the existing
LinerSearch class, which is not worth the effort for this example. The current
implementation is sufficient for educational purposes, and it is easy to
understand and use.
*/
class GradientsAD
{
public:
  using Scalar = apsc::LineSearch_traits::Scalar;
  using Vector = apsc::LineSearch_traits::Vector;
  using Matrix = apsc::LineSearch_traits::Matrix;
  using CostFunction = apsc::LineSearch_traits::CostFunction;
  using Gradient = apsc::LineSearch_traits::Gradient;
  using Hessian = apsc::LineSearch_traits::Hessian;
  using AutodiffScalar = autodiff::var;
  using AutoDiffArgumentType = autodiff::VectorXvar;
  using AutoDiffReturnType = AutodiffScalar;
  using AutodiffCostFunction =
    std::function<AutoDiffReturnType(AutoDiffArgumentType const &)>;

  template <typename CostFunction>
  GradientsAD(CostFunction &&costFunction)
    : costFunction{std::forward<CostFunction>(costFunction)}
  {}
  /*!
    \brief Evaluate the cost function at the given point x.
    \param x The point at which to evaluate the cost function.
    \return The value of the cost function at x.
  */
  Scalar
  evaluateCost(const Vector &x) const
  {
    AutoDiffArgumentType xad = x;
    costValue = costFunction(xad);
    return autodiff::val(costValue);
  }
  /*!
    \brief Evaluate the gradient of the cost function at the given point x.
    \param x The point at which to evaluate the gradient.
    \return The gradient of the cost function at x.
  */
  Vector
  evaluateGradient(const Vector &x) const
  {
    AutoDiffArgumentType xad = x;
    costValue = costFunction(xad);
    return autodiff::gradient(costValue, xad);
  }
  /*!
    \brief Evaluate the Hessian of the cost function at the given point x.
    \param x The point at which to evaluate the Hessian.
    \return The Hessian of the cost function at x.
    \note This function must be called after evaluateCost, as it uses the
    internal state set by that function.
  */
  Matrix
  evaluateHessian(const Vector &x) const
  {
    AutoDiffArgumentType xad = x;
    costValue = costFunction(xad);
    Vector grad;
    return autodiff::hessian(costValue, xad, grad);
  }
  /*!
  \brief Computes all the cost, gradient and Hessian at the given point x.
  \param x The point at which to evaluate the cost, gradient and Hessian.
  \return A tuple containing the cost, gradient and Hessian at x.
  */
  std::tuple<Scalar, Vector, Matrix>
  evaluateAll(const Vector &x) const
  {
    AutoDiffArgumentType xad = x;
    costValue = costFunction(xad);
    Vector grad;
    Matrix hess = autodiff::hessian(costValue, xad, grad);
    return std::make_tuple(autodiff::val(costValue), grad, hess);
  }
  /*!
      \brief Get the a callable for the cost function.
      \return A callable object encapsulating th cost function.
    */
  auto
  theCostFunction() const
  {
    return [this](const Vector &x) { return this->evaluateCost(x); };
  }
  /*!
    \brief Get the a callable for the gradient function.
    \return A callable object encapsulating the gradient function.
  */
  auto
  theGradientFunction() const
  {
    return [this](const Vector &x) { return this->evaluateGradient(x); };
  }
  /*!
    \brief Get the a callable for the Hessian function.
    \return A callable object encapsulating the Hessian function.
  */
  auto
  theHessianFunction() const
  {
    return [this](const Vector &x) { return this->evaluateHessian(x); };
  }

private:
  mutable AutodiffCostFunction costFunction;
  mutable AutoDiffReturnType   costValue;
};
} // namespace apsc
#pragma GCC diagnostic pop