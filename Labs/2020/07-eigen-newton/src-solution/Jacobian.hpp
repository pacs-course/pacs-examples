#ifndef JACOBIAN_HPP
#define JACOBIAN_HPP

#include "NewtonTraits.hpp"

/// The base class for the Jacobian.
/// It inherits from NonLinSolveTraits to get all types defined
/// there
///
/// It implements the basic methods for the application of the
/// Jacobian, which in fact may be a "pseudoJacobian" for
/// quasi-Newton methods or evne Picard iterations
///
class JacobianBase : public NewtonTraits
{
public:
  /// Takes a pointer to an existing non linear system
  JacobianBase(NonLinearSystemType const *nls = nullptr)
    : M_sys{nls} {};

  /// Solves the system \f$J^{-1}(x) b\f$
  /// /param x the point where to evaluate the Jacobian
  /// /param b the right hand side of the system J(x) y=b
  /// /return the result
  virtual ArgumentType
  solve(const ArgumentType &x, const ArgumentType &b) const = 0;

  /// Sets the non linear system.
  void
  setNonLinSys(const NonLinearSystemType *s)
  {
    M_sys = s;
  };

  virtual ~JacobianBase() = default;

protected:
  /// The non-linear system
  const NonLinearSystemType *M_sys;
};

/// Computes the jacobian by finite differences.
/**
 * The final keyword has been introduced in C++11 to stop a
 * hierarchy. No class can be derived from it.
 *
 * @note This implementation is mildly dangerous, since it stores a pointer
 * to the linear system and nobody ensures that the linear system will
 * not be destroyed before the call method of this class is used! But
 * we may consider it a very unlikely event.
 */
class DiscreteJacobian final : public JacobianBase
{
public:
  /// Constructor optionally takes the pointer to a non linear
  /// system.
  DiscreteJacobian(const NonLinearSystemType *sys = nullptr,
                   double                     tol = 1e-4)
    : JacobianBase{sys}
    , M_tol{tol} {};

  /// Solves the system.
  ///
  /// Computes the Jacobian using finite differences.
  /// /param x the point where to evaluate the Jacobian
  /// /param b the right hand side of the system J(x) y=b
  /// /return the result
  ArgumentType
  solve(const ArgumentType &x, const ArgumentType &b) const override;

private:
  /// The current finite difference step.
  double M_tol;
};

/// Here I explicitly set the Jacobian as a function
class FullJacobian : public JacobianBase
{
public:
  /// Inherit base constructor
  using JacobianBase::JacobianBase;

  FullJacobian(const FullJacobian &) = default;
  FullJacobian(FullJacobian &&)      = default;

  FullJacobian &
  operator=(const FullJacobian &) = default;

  FullJacobian &
  operator=(FullJacobian &&) = default;

  /// I can pass the JacobianFunction via constructor
  /// \param j an object convertible to a JacobianFunction
  FullJacobian(const JacobianFunctionType &j,
               const NonLinearSystemType * nls = nullptr)
    : JacobianBase{nls}
    , Jac{j} {};

  /// Solve \f$J(x) d = - b\f$
  /// \param x The point to compute residual
  /// \param b The right hand side
  /// \return The result of the application of the inverse Jacobian
  /// to the residual
  ArgumentType
  solve(const ArgumentType &x, const ArgumentType &b) const override;

private:
  JacobianFunctionType Jac;
};

#endif /* JACOBIAN_HPP */
