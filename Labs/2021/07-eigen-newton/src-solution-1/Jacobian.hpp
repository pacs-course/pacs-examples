#ifndef JACOBIAN_HPP
#define JACOBIAN_HPP

#include "NewtonTraits.hpp"

/// The base class for representing the Jacobian.
///
/// It implements the basic methods for the application of the
/// Jacobian, which in fact may be a "pseudo-Jacobian" for
/// quasi-Newton methods or even Picard iterations.
class JacobianBase
{
public:
  /// Short-hand alias.
  using T = NewtonTraits;

  /// Default constructor.
  JacobianBase() = default;

  /// Solve J(x) * delta_x = res.
  virtual T::VariableType
  solve(const T::VariableType &x, const T::VariableType &res) const = 0;

  /// Destructor. Needed since this is a pure virtual class.
  virtual ~JacobianBase() = default;
};

/// Computes the jacobian by finite differences.
class DiscreteJacobian final : public JacobianBase
{
public:
  /// Constructor.
  DiscreteJacobian(const T::NonLinearSystemType &system_, const double &h_)
    : system(system_)
    , h(h_)
  {}

  /// Override of the base class method.
  virtual T::VariableType
  solve(const T::VariableType &x, const T::VariableType &res) const override;

private:
  /// Non-linear system.
  T::NonLinearSystemType system;

  /// Finite differences step.
  double h;
};

/// Jacobian function.
class FullJacobian final : public JacobianBase
{
public:
  /// Constructor.
  FullJacobian(const T::JacobianFunctionType &jac_)
    : jac(jac_){};

  /// Override of the base class method.
  virtual T::VariableType
  solve(const T::VariableType &x, const T::VariableType &res) const override;

private:
  /// Jacobian function.
  T::JacobianFunctionType jac;
};

#endif /* JACOBIAN_HPP */
