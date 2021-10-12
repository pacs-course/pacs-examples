/*
 * GradientFiniteDifference.hpp
 *
 *  Created on: Dec 29, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_LINESEARCH_GRADIENTFINITEDIFFERENCE_HPP_
#define EXAMPLES_SRC_LINESEARCH_GRADIENTFINITEDIFFERENCE_HPP_
#include "Eigen/Dense"
#include "LineSearch_traits.hpp"
#include <cmath>
#include <limits>
namespace apsc
{
/*!
 * Computing the gradient via finite differences. This enum class determines the
 * type of differencing.
 */
enum class FiniteDifferenceType
{
  Centered,
  Forward,
  Backward
};

/*!
 * Computes gradients of a function Rn->R via finite differences
 *
 * @tparam FDT The finite difference type to be used for computing the
 * approximate gradient
 */
template <FiniteDifferenceType FDT = FiniteDifferenceType::Centered>
class GradientFiniteDifference
{
public:
  using Scalar = LineSearch_traits::Scalar;
  using Vector = LineSearch_traits::Vector;
  GradientFiniteDifference() = default;
  //! Here you pass the function to be derived.
  GradientFiniteDifference(const LineSearch_traits::CostFunction &f) : f(f){};
  //! Here you change the function to be derived.
  void
  setFunction(const LineSearch_traits::CostFunction &ff)
  {
    f = ff;
  }
  Vector operator()(Vector const &x);

private:
  LineSearch_traits::CostFunction f;
  //! A small number used for differencing
  double const smallNumber = std::sqrt(std::numeric_limits<double>::epsilon());
};

template <FiniteDifferenceType FDT>
typename GradientFiniteDifference<FDT>::Vector
GradientFiniteDifference<FDT>::operator()(const Vector &x)
{
  auto   n = x.size();
  Vector res(n);
  auto   h = std::max(smallNumber, x.norm() * smallNumber);
  Vector e = Vector::Zero(n);
  if constexpr(FDT == FiniteDifferenceType::Centered)
    {
      auto h2 = 1. / (2.0 * h);
      for(decltype(n) i = 0u; i < n; ++i)
        {
          e(i) = h;
          res(i) = h2 * (f(x + e) - f(x - e));
          e(i) = 0.;
        }
    }
  else if constexpr(FDT == FiniteDifferenceType::Forward)
    {
      auto f0 = f(x);
      auto h2 = 1. / h;
      for(std::size_t i = 0u; i < n; ++i)
        {
          e(i) = h;
          res(i) = (f(x + e) - f0) * h2;
          e(i) = 0;
        }
    }
  else
    {
      auto f0 = f(x);
      auto h2 = 1. / h;
      for(std::size_t i = 0u; i < n; ++i)
        {
          e(i) = h;
          res(i) = (f0 - f(x - e)) * h2;
          e(i) = 0.;
        }
    }
  return res;
}

} // namespace apsc

#endif /* EXAMPLES_SRC_LINESEARCH_GRADIENTFINITEDIFFERENCE_HPP_ */
