/*
 * CostFunction.hpp
 *
 *  Created on: Jan 25, 2019
 *      Author: forma
 */

#ifndef SRC_REGRESSION_MSECOSTFUNCTION_HPP_
#define SRC_REGRESSION_MSECOSTFUNCTION_HPP_
#include "PolynomialRegressionEvaluator.hpp"
#include "RegressionTraits.hpp"
#include <utility>
namespace LinearAlgebra
{
//! Mean Square Error Cost function
/*
 * Defines the basic  interface of a class that implements a cost function
 * based on Mean Square error
 * \tparam ModelEvaluator A class that evaluates a model
 */
template <class ModelEvaluator> class MSECostFunction
{
public:
  // I am using the same trait of modelEvalutor
  using Trait = typename ModelEvaluator::Trait;
  using Parameters = typename Trait::Parameters;
  using Vector = typename Trait::Vector;
  //! Constructor that takes a ModelEvaluator in input
  //
  MSECostFunction(ModelEvaluator const &m) : M_model(m) {}
  //! Constructor thet moves a ModelEvaluator
  MSECostFunction(ModelEvaluator &&m) : M_model(std::move(m)) {}
  //! Default constructor
  MSECostFunction() = default;
  //! To set a model
  template <class M>
  void
  setModel(M &&m)
  {
    M_model = std::forward<M>(m);
  }
  //! Get the model evaluator
  auto
  get_model() const
  {
    return M_model;
  }
  //! Computes the cost function given by \$f c=||X - Y||^2\f$
  /**
   * @param X vector of data point
   * @param Y vector of values
   * @param model The underlying model
   * @param parameters The vector of parameters
   */
  double
  eval(Vector const &X, Vector const &Y, Parameters const &parameters) const
  {
    double sum = 0.0;
    for(int i = 0; i < X.size(); ++i)
      {
        auto val = M_model.eval(X(i), parameters) - Y(i);
        sum += val * val;
      }
    return 0.5 * sum / X.size();
  }
  //! Gradiens w.r.t. parameters
  Vector
  evalDerPar(Vector const &X, Vector const &Y,
             Parameters const &parameters) const
  {
    Vector res = Vector::Zero(parameters.size());
    for(int i = 0; i < X.size(); ++i)
      {
        res += (M_model.eval(X(i), parameters) - Y(i)) *
               M_model.evalDerPar(X(i), parameters);
      }
    return res / X.size();
  }

  //! Get the underlying Model Evaluator
  /*!
   * A classic getter. It returns a copy of the underlying model evaluator and
   * is declared const. Which means that can be used only in a constant
   * expression or as rvalue.
   *
   * @return the model
   */
  ModelEvaluator
  getModel() const
  {
    return M_model;
  }
  //! This is an alternative way to have a setter
  /*!
   * It relies on function overloading
   * This version will be used in a non-const lvalue expression.
   * It can be used only on non-const CostFunction object since it is not const
   *
   * @return
   */
  ModelEvaluator &
  getModel()
  {
    return M_model;
  }

  //! This is more classic setter
  /*!
   * @param model the model evaluator to set
   */
  void
  setModel(ModelEvaluator const &model)
  {
    M_model = model;
  }

private:
  ModelEvaluator M_model;
};
} // namespace LinearAlgebra

#endif /* SRC_REGRESSION_MSECOSTFUNCTION_HPP_ */
