/*
 * RegressionSolver.hpp
 *
 *  Created on: Jan 25, 2019
 *      Author: forma
 */

#ifndef SRC_REGRESSION_REGRESSIONSOLVER_HPP_
#define SRC_REGRESSION_REGRESSIONSOLVER_HPP_
#include "Eigen/Core"
#include "Eigen/QR"

#include <cassert>
#include <concepts>
#include <utility>

#include "MSECostFunction.hpp"
#include "PolynomialRegressionEvaluator.hpp"
namespace LinearAlgebra
{
//! Computes the parameters that fit a set of data
/*!
 * It can be applied only on models of the form
 * \f[
 * f(x;\beta)=\sum_k \beta_k\phi_k(x)
 * \f]
 *
 * and computes the minimum of the MSE cost function given the training points
 * (X,Y):
 *
 * \f[
 * J(X,Y)=\sum_{i=1}^N (y_i-f(x_1))^2
 * \f]
 *
 * We use QR factorization of the matrix of the corresponding normal system.
 * This is very efficient if the data set, lets say \f$ N<100\f$.
 *
 * @tpar Model a model evaluator that complies with what written above
 */
template <class Model> class LinearRegressionSolverMSE
{
public:
  using Trait = typename Model::Trait;
  using Vector = typename Trait::Vector;
  using Parameters = typename Trait::Parameters;
  using Matrix = typename Trait::Matrix;
  //! A can take a ModelEvaluator as input
  explicit LinearRegressionSolverMSE(Model const &m) : M_model(m) {}
  //! A constructor that moves the mode (if possible)
  explicit LinearRegressionSolverMSE(Model &&m) : M_model(std::move(m)) {}
  //! Default constructor
  LinearRegressionSolverMSE() = default;

  //! Setter that takes anything that can be converted to a model evaluator
  /*!
   * and moves it if possible!
   *
   * @param m The model to store
   */
  template <class M>
    requires std::assignable_from<Model &, M &&>
  void
  setModel(M &&m)
  {
    M_model = std::forward<M>(m);
  }
  //! Get the underlying model
  /*!
   * @return the model
   */
  [[nodiscard]] auto const &
  getModel() const
  {
    return M_model;
  }
  //! A different version of a setter.
  /*!
   *
   * @return A reference to the model
   *
   * @note compared to the version above, this method does not allow to move
   * into the changed model. A copy operation is performed (unless the compiler
   * is very smart).
   *
   * \code
   * a.getmodel()=functioncreatingmodel();
   * \endcode
   */
  [[nodiscard]] auto &
  getModel()
  {
    return M_model;
  }

  [[nodiscard]] Parameters
  solve(Vector const &X, Vector const &Y) const
  {
    assert(X.size() == Y.size() && "X and Y must have the same size");
    assert(X.size() > 0 && "At least one training point is required");
    // Get the basis functions
    auto const &modelBasis = M_model.getModelBasis();
    // Build Matrix
    // I use the Hauseholder QR factorization of Eigen
    Matrix A(X.size(), M_model.size());
    for(Eigen::Index k = 0; k < X.size(); ++k) // row index
      {
        A.row(k) = modelBasis.eval(X(k)).transpose();
      }
    return A.householderQr().solve(Y).eval();
  }

private:
  Model M_model;
};

} // namespace LinearAlgebra

#endif /* SRC_REGRESSION_REGRESSIONSOLVER_HPP_ */
