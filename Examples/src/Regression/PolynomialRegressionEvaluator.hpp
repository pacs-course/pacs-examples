/*
 * Evaluators.hpp
 *
 *  Created on: Jan 25, 2019
 *      Author: forma
 */

#ifndef SRC_REGRESSION_POLYNOMIALREGRESSIONEVALUATOR_HPP_
#define SRC_REGRESSION_POLYNOMIALREGRESSIONEVALUATOR_HPP_
#include "RegressionTraits.hpp"

#include <utility>

#include "PolynomialBasis.hpp"
namespace LinearAlgebra
{
//! This class implements the evaluation of a model
/*
 *  This version assumes that the model is a linear combination
 *  of basis function.
 */
template <class ModelBasis> class PolynomialRegressionEvaluator
{
public:
  // I use the same Linear Algebra Library of the ModelBasis
  using Trait = typename ModelBasis::Trait;
  using Parameters = typename Trait::Parameters;
  using Vector = typename Trait::Vector;
  //! The constructor that takes a well formed ModelBasis object
  explicit PolynomialRegressionEvaluator(ModelBasis const &modelbasis)
    : M_basis{modelbasis}
  {}
  //! Move constructor for the basis object
  explicit PolynomialRegressionEvaluator(ModelBasis &&modelbasis)
    : M_basis{std::move(modelbasis)}
  {}
  //! Default constuctor
  PolynomialRegressionEvaluator() = default;

  //! evaluate at point x
  /*!
   * They must however comply with some rule
   * @param x The evaluation point
   * @param parameters, an object with the method dot()
   * @note the size of parameters must match the number of basis functions in
   * ModelBasis
   */
  [[nodiscard]] double
  eval(double x, Parameters const &parameters) const
  {
    // dot is a method of Eigen vectors so model.eval() and parameters should be
    // Eigen vectors
    return parameters.dot(M_basis.eval(x));
  }
  //! Evaluates derivative w.r.t. x
  /*!
   * Not really necessary for our purposes. Only for completeness
   */
  [[nodiscard]] double
  evalDerx(double x, Parameters const &parameters) const
  {
    // dot is a method of Eigen vectors so model.eval() and parameters should be
    // Eigen vectors
    return parameters.dot(M_basis.derivatives(x));
  }
  //! Derivative wrt the parameters
  /*!
   * @note Only required if we use a gradient scheme to compute the optimal
   * parameters
   */
  [[nodiscard]] Vector
  evalDerPar(double x, [[maybe_unused]] Parameters const &parameters) const
  {
    return M_basis.eval(x);
  }
  //! Get the basis functions
  [[nodiscard]] ModelBasis const &
  getModelBasis() const
  {
    return M_basis;
  }
  //! Get model basis so that they can be modified
  /*!
   *  This is a different way of making a setter!
   *  I relies on function overloading.
   */
  [[nodiscard]] ModelBasis &
  getModelBasis()
  {
    return M_basis;
  }

  //! The capacity of the underlying model
  [[nodiscard]] std::size_t
  size() const noexcept
  {
    return M_basis.size();
  }

private:
  ModelBasis M_basis;
};
} // namespace LinearAlgebra

#endif /* SRC_REGRESSION_POLYNOMIALREGRESSIONEVALUATOR_HPP_ */
