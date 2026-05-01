/*
 * CostFunctionProxyCppNumSolver.hpp
 *
 *  Created on: Jan 27, 2019
 *      Author: forma
 */

#ifndef SRC_REGRESSION_COSTFUNCTIONPROXYCPPNUMSOLVER_HPP_
#define SRC_REGRESSION_COSTFUNCTIONPROXYCPPNUMSOLVER_HPP_

#include "MSECostFunction.hpp"
#include "cppoptlib/problem.h" // from CppNumericalSolvers

#include <iomanip>
#include <iostream>
#include <type_traits>

namespace LinearAlgebra
{
/*! \file
 * Front-end adapter for the optional CppNumericalSolvers library.
 *
 * This header wraps a regression cost function so it satisfies the interface
 * expected by `cppoptlib::Problem<double>`.
 *
 * To use it, initialize the submodule and install the headers in the PACS
 * include tree:
 *
 * git submodule update --init --recursive
 * cd Examples/src/LinearAlgebra/CppNumericalSolvers
 * make
 *
 * If CppNumericalSolvers is not available, compile the regression example with
 * `NOCPPSOLVER` defined so that the optimization-based path in `main.cpp` is
 * skipped.
 */

/*!
 * \brief Adapter exposing a regression cost function to CppNumericalSolvers.
 *
 * \tparam CostFunction Cost function type. It must provide:
 * - `double eval(X, Y, parameters) const`
 * - `Vector evalDerPar(X, Y, parameters) const`
 *
 * The wrapped cost function must use the Eigen-based regression trait because
 * CppNumericalSolvers expects Eigen vectors.
 */
template <class CostFunction>
class CostFunctionProxyCppSolver : public cppoptlib::Problem<double>
{
public:
  using Trait = typename CostFunction::Trait;
  using CorrectTrait = LinearAlgebra::RegressionTraits<LinearAlgebra::EIGEN>;
  using Parameters = typename CorrectTrait::Parameters;
  using Vector = typename CorrectTrait::Vector;

  static_assert(std::is_same_v<Trait, CorrectTrait>,
                "The cost function for CppSolver must use Eigen library");

  //! Build the proxy on top of an existing cost function and data set.
  /*!
   * All arguments are stored by reference, so the caller must ensure that the
   * referenced objects outlive the proxy.
   *
   * \param c The cost function
   * \param X The vector of independent variables
   * \param Y The corresponding values
   */
  CostFunctionProxyCppSolver(CostFunction const &c, Vector const &X,
                             Vector const &Y)
    : M_cost{c}, M_X{X}, M_Y{Y}
  {}

  //! Computes the cost function value at the parameter vector `p`.
  double
  value(Parameters const &p) override
  {
    return M_cost.eval(M_X, M_Y, p);
  }

  //! Computes the gradient with respect to the parameter vector.
  /*!
   * \param p The current parameter vector
   * \param grad Output gradient
   */
  void
  gradient(Parameters const &p, Vector &grad) override
  {
    grad = M_cost.evalDerPar(M_X, M_Y, p);
  }

  // Iteration trace enabled only in debug builds.
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
