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
  LinearRegressionSolverMSE(const Model &m) : M_model(m) {}
  //! A constructor that moves the mode (if possible)
  LinearRegressionSolverMSE(Model &&m) : M_model(std::move(m)) {}
  //! Default constructor
  LinearRegressionSolverMSE() = default;

  //! Setter that takes anything that can be converted to a model evaluator
  /*!
   * and moves it if possible!
   *
   * @param m The model to store
   */
  template <class M>
  void
  setModel(M &&m)
  {
    M_model = std::forward<M>(m);
  }
  //! Get the underlying model
  /*!
   * @return the model
   */
  auto
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
  auto &
  getModel()
  {
    return M_model;
  }

  Parameters
  solve(Vector const &X, Vector const &Y) const
  {
    // Get the basis functions
    auto const &modelBasis = M_model.getModelBasis();
    // Build Matrix
    // I use the Hauseholder QR factorization of Eigen
    Matrix A(X.size(), M_model.size());
    for(std::size_t k = 0; k < X.size(); ++k) // row index
      {
        Vector row = modelBasis.eval(X(k));
        A.row(k) = row.transpose();
      }
    Parameters res = (A.householderQr().solve(Y)).eval();
    return res;
    // Potentially memory saving
    // return  A.householderQr().solve(Y);
  }

private:
  Model M_model;
};

} // namespace LinearAlgebra

#endif /* SRC_REGRESSION_REGRESSIONSOLVER_HPP_ */
