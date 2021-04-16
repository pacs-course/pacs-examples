#ifndef JACOBIAN_HPP
#define JACOBIAN_HPP

#include "NewtonTraits.hpp"

/// The base class for representing the Jacobian.
///
/// It implements the basic methods for the application of the
/// Jacobian, which in fact may be a "pseudo-Jacobian" for
/// quasi-Newton methods or even Picard iterations.
template <ProblemType Type>
class JacobianBase
{
public:
  /// Short-hand alias.
  using T = NewtonTraits<Type>;

  /// Default constructor.
  JacobianBase() = default;

  /// Solve J(x) * delta_x = res.
  virtual typename T::VariableType
  solve(const typename T::VariableType &x,
        const typename T::VariableType &res) const = 0;

  /// Destructor. Needed since this is a pure virtual class.
  virtual ~JacobianBase() = default;
};

/// Computes the jacobian by finite differences.
template <ProblemType Type>
class DiscreteJacobian final : public JacobianBase<Type>
{
public:
  /// Short-hand alias.
  using T = NewtonTraits<Type>;

  /// Constructor.
  DiscreteJacobian(const typename T::NonLinearSystemType &system_,
                   const double &                         h_)
    : system(system_)
    , h(h_)
  {}

  /// Override of the base class method.
  virtual typename T::VariableType
  solve(const typename T::VariableType &x,
        const typename T::VariableType &res) const override;

private:
  /// Non-linear system.
  typename T::NonLinearSystemType system;

  /// Finite differences step.
  double h;
};

/// Jacobian function.
template <ProblemType Type>
class FullJacobian final : public JacobianBase<Type>
{
public:
  /// Short-hand alias.
  using T = NewtonTraits<Type>;

  /// Constructor.
  FullJacobian(const typename T::JacobianFunctionType &jac_)
    : jac(jac_){};

  /// Override of the base class method.
  virtual typename T::VariableType
  solve(const typename T::VariableType &x,
        const typename T::VariableType &res) const override;

private:
  /// Jacobian function.
  typename T::JacobianFunctionType jac;
};

#endif /* JACOBIAN_HPP */
