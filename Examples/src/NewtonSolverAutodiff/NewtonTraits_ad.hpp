/*! \file NewtonTraits.hpp
 *  \brief Types used by the Newton solver with automatic differentiation.
 */
#ifndef NEWTON_SOLVER_AUTODIFF_TRAITS_HPP
#define NEWTON_SOLVER_AUTODIFF_TRAITS_HPP

#pragma GCC diagnostic ignored "-Wdeprecated"
#pragma GCC diagnostic ignored "-Wsuggest-override"
#pragma GCC diagnostic ignored "-Wparentheses"
#include <Eigen/Dense>
#include <autodiff/forward/real/eigen.hpp>
#include <functional>

namespace apsc
{
// namespace to avoid clashes with NewtonSOlver in
// Examples/src/NewtonSolver/Newton.hpp
namespace newton_ad
{
  //! Traits for a Newton solver whose residual is differentiable by autodiff.
  struct NewtonTraits
  {
    //! The type of the argument of the non linear system from the client side
    using ArgumentType =
      Eigen::VectorXd; // The type of the return value of the non linear system
                       // from the client side
    using ReturnType = ArgumentType;
    // The type of the argument of the non linear system from the autodiff side
    using AutoDiffArgumentType = autodiff::VectorXreal;
    // The type of the return value of the non linear system from the autodiff
    // side
    using AutoDiffReturnType = AutoDiffArgumentType;
    //! Write the residual using autodiff scalar operations (sin, cos, exp,
    //! ...).
    using NonLinearSystemType =
      std::function<AutoDiffReturnType(const AutoDiffArgumentType &)>;
    using JacobianMatrixType = Eigen::MatrixXd;
  };

} // namespace newton_ad
} // namespace apsc

// Helper namespace if you want to simplify things in the client code
namespace newton_ad = apsc::newton_ad;

#endif
