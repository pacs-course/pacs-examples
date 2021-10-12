/*
 * CostFunctionProxyCppNumSolver.hpp
 *
 *  Created on: Jan 27, 2019
 *      Author: forma
 */

#ifndef SRC_REGRESSION_COSTFUNCTIONPROXYCPPNUMSOLVER_HPP_
#define SRC_REGRESSION_COSTFUNCTIONPROXYCPPNUMSOLVER_HPP_
#include "MSECostFunction.hpp"
#include "cppoptlib/problem.h" // from CppNumerical Solvers
#include <iomanip>
#include <iostream>
#include <type_traits>

namespace LinearAlgebra
{
/*! \file
 * This utility is a front end to the software in CppNumericalSolvers,
 * which provides tools for minimization of univariate and multivatiate
 * functions.
 * To use it you have first to go into
 * Examples/src/LinearAlgebra/CppNumericalSolvers
 * and type
 * make
 * In this way, all relevant files are copied in the include/ directory
 * used for the PACS Examples.
 * Note that CppNumericalSolvers is in fact a submodule. If you have not
 * already got it, just type
 *
 * git submodule update --init --recursive
 */

/*!
 * \brief This class defines a proxy for a Cost function so you may use it
 with any optimization technique provided in that library.

 \tpar CostFunction A cost function. It should have a method called eval
 taking the points and the value of the parameters.
 */
template <class CostFunction>
class CostFunctionProxyCppSolver : public cppoptlib::Problem<double>
{
public:
  using Trait = typename CostFunction::Trait;
  using correctTrait = LinearAlgebra::RegressionTraits<LinearAlgebra::EIGEN>;
  // cpp colvers use Eigen so I cannot use anything else
  static_assert(std::is_same<Trait, correctTrait>::value,
                "The cost function for CppSolver must use Eigen library");
  // Luckily CppNumericalSolvers uses the
  // same type for the argument of the cost function
  /*!
   * @note As long as Parameters and Vector are both aliases to Eigen::VectorXd
   */
  using Parameters = typename correctTrait::Parameters;
  using Vector = typename correctTrait::Vector;
  //! This is a proxy. It is build on top of an existing CostFunction object.
  /*!
   * I need to pass everything needed to evaluate the cost function
   * Internally all arguments are stored by reference, so there is no waste of
   * memory
   * \param c The cost function
   * \param X The vector of independent variables
   * \param Y The corresponding values
   *
   */
  template <class Vector>
  CostFunctionProxyCppSolver(CostFunction const &c, Vector const &X,
                             Vector const &Y)
    : M_cost{c}, M_X{X}, M_Y{Y}
  {}

  //! Computes f(x)
  double
  value(Parameters const &p) override
  {
    return M_cost.eval(M_X, M_Y, p);
  }

  //! The gradient of f is returned through the non const reference
  /*!
   * @param p the variable w.r.t. which we are optimizing. In this case the
   * parameters of the more.
   * @param grad the gradient
   */
  void
  gradient(Parameters const &p, Vector &grad) override
  {
    grad = M_cost.evalDerPar(M_X, M_Y, p);
  }
  // This is to have some results on the iteration activate only when debugging
  // debugging is controlled by the NDEBUG flag. If defined I do not want
  // debugging

#ifndef NDEBUG
  bool
  callback(const cppoptlib::Criteria<double> &state, const Vector &x) override
  {
    std::cout << "(" << std::setw(2) << state.iterations << ")"
              << " ||dx|| = " << std::fixed << std::setw(8)
              << std::setprecision(4) << state.gradNorm
              << " ||x|| = " << std::setw(6) << x.norm()
              << " f(x) = " << std::setw(8) << value(x) << " x = ["
              << std::setprecision(8) << x.transpose() << "]" << std::endl;
    return true;
  }
#endif
private:
  CostFunction const &M_cost;
  Vector const &      M_X;
  Vector const &      M_Y;
};
} // namespace LinearAlgebra

#endif /* SRC_REGRESSION_COSTFUNCTIONPROXYCPPNUMSOLVER_HPP_ */
