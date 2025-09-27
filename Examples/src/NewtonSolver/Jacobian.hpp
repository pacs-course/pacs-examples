/*
 * Jacobian.hpp
 *
 *  Created on: Feb 21, 2020
 *      Author: forma
 */

#ifndef NONLINSYSSOLVER_JACOBIAN_HPP_
#define NONLINSYSSOLVER_JACOBIAN_HPP_
#include "NewtonTraits.hpp"
namespace apsc
{
//! @brief
//! The base class for the Jacobian.
//! It inherits from NonLinSolveTraits to get all types defined there
//! This is possible since the trait is not a template.
//! It implements the basic methods for the application of the Jacobian, which
//! in fact may be a "pseudoJacobian" for quasi-Newton methods or even Picard
//! iterations
//!
class JacobianBase : public NewtonTraits
{
public:
  //! Takes a pointer to an existing non linear system
  JacobianBase(NonLinearSystemType const *nls = nullptr) : M_sys{nls} {};
  //! Solves the system \f$J(x)z= b\f$
  //! /param x the point where to evaluate the Jacobian
  //! /param b the right hand side of the system J(x) y=b
  //! /return the result z
  virtual ArgumentType solve(ArgumentType const &x,
                             ArgumentType const &b) const = 0;
  //! Sets the non linear system.
  virtual void
  setNonLinSys(NonLinearSystemType const *s)
  {
    M_sys = s;
  };
  virtual ~JacobianBase(){};

protected:
  //! A pointer to the non-linear system
  NonLinearSystemType const *M_sys;
};

//! @brief Computes the jacobian by finite differences.
/*!
  @details Final class that implements the following approximation of
  the Jacobian

  \f[
      J = (F(x+h)-F(x-h))/2|h| + \lambda I
  \f]

  where \f$\lambda I\f$ is a possible regularization.

  @note This implementation is mildly dangerous, since it stores a pointer
  to the linear system and nobody ensures that the linear system will not
  be destroyed before the call method of this class is used! But we may consider
  it a very unlikely event. So a prerequisite is that
  the non linear system is existing and accessible.
  An alternative is to use a reference. But a pointer
  is the simplest solution.
 */
class DiscreteJacobian final : public JacobianBase
{
public:
  //! Constructor optionally takes the pointer to a non linear system.
  /*!
   * @param sys The non linear system
   * @param spacing The spacing |h| for the computation of the approximate
   * derivative
   */
  DiscreteJacobian(NonLinearSystemType const *sys = nullptr,
                   double spacing = defaultTol, double lambda = defaultLambda)
    : JacobianBase{sys}, tol{spacing}, lambda{lambda} {};
  //! Solves the system
  /*!
   * @param x the point where to evaluate the Jacobian
   * @param b he right hand side of the system J(x) y=b
   * @return The result
   */
  //!
  ArgumentType solve(ArgumentType const &x,
                     ArgumentType const &b) const override;

  /*!
    @brief Set the parameters for the computation of the Jacobian
    @param tol The spacing |h| for the computation of the approximate derivative
    @param lambda The regularization parameter
  */
  void
  setParameters(double tol = defaultTol, double lambda = defaultLambda)
  {
    this->tol = tol;
    this->lambda = lambda;
  }
  //! The current spacing |h|
  double tol = defaultTol;
  //! Regularization (if needed)
  double lambda = defaultLambda;

private:
  //! Default tolerance.
  static double constexpr defaultTol = 1e-4;
  static double constexpr defaultLambda = 0.0;
};

/*!
 * The identityJacobian just scales the residual by a factor defaulted to 1
 * by default \f$\lambda\f$ is one.
 * It applies \f$ J=\lambda^{-1}Ib\f$
 */
class IdentityJacobian final : public JacobianBase
{
public:
  using JacobianBase::JacobianBase;
  //! Returns the rhs b scaled by lambda
  //! /param x (not used)
  //! /param b the right hand side of the system \f$\alpha^{-1} I y=b\f$
  //! /return the result

  ArgumentType
  solve(ArgumentType const & /*x*/, ArgumentType const &b) const override
  {
    return this->lambda * b;
  }
  //! Set the scaling factor
  //! /param l the scaling factor
  void
  setLambda(double l)
  {
    lambda = l;
  }
  //! The scaling factor
private:
  double lambda = 1.0;
};

//! Here I explicitly set the Jacobian as a function
class FullJacobian : public JacobianBase
{
public:
  //! Inherit base constructor
  using JacobianBase::JacobianBase;
  FullJacobian(FullJacobian const &) = default;
  FullJacobian(FullJacobian &&) = default;
  FullJacobian &operator=(FullJacobian const &) = default;
  FullJacobian &operator=(FullJacobian &&) = default;

  //! @brief I can pass the JacobianFunction via constructor
  //! @param j an object convertible to a JacobianFunction
  FullJacobian(JacobianFunctionType const &j,
               NonLinearSystemType  const      *nls = nullptr)
    : JacobianBase{nls}, Jac{j} {};

  //! I can change the JacobianFunction
  //! \tparam J a type convertible to JacobianFunction
  //! \param j an object convertible to a JacobianFunction
  template <class J>
  void
  setJacobianFunction(J &&j)
  {
    Jac = std::forward<J>(j);
  }
  //! I may get a copy of the jacobianFunction
  //! \return A copy of the stored JacobianFunction
  JacobianFunctionType
  getJacobianFunction() const
  {
    return Jac;
  }
  //! Solve \f$J(x) d = - b\f$
  //! \param x The point to compute residual
  //! \param b The right hand side
  //! \return The result of the application of the inverse Jacobian to the
  //! residual
  ArgumentType solve(ArgumentType const &x,
                     ArgumentType const &b) const override;

private:
  JacobianFunctionType Jac;
};

//! Implements Broyden (bad) method
//!
//! The so called Broyden bad method (which is not so bad after all) is a
//! quasi-Newton method that uses a formula for updating an approximation of the
//! inverse of the Jacobian. The formula makes use of the last two values of the
//! solution and of the value of the function so that the computed approximation
//! converges to the actual Jacobian
//!
class BroydenB : public JacobianBase
{
public:
  //! If no initial matrix is give, we take the identity.
  //!
  //! Since I do not know at this stage the dimension of the
  //! system, the actual computation of the identity matrix is done
  //! the first time solve() is called.
  using JacobianBase::JacobianBase;
  //!
  //! I have to give an inital matrix, approximation of the Jacobian
  //!
  //! \param B the initial approximation of the Jacobian
  BroydenB(JacobianMatrixType const &B) : B{B}, firstTime{true} {}
  //!
  //! Sets the initial matrix, approximation of the Jacobian.
  //!
  //! It can be used also for restarting purposes. When given the matrix
  //! the method restarts from the new iteration given in the method solve().
  //!
  //! \param newB the initial approximation of the Jacobian. If not provided,
  //!        a default-constructed JacobianMatrixType is used, which may be a
  //!        zero or identity matrix depending on the implementation of
  //!        JacobianMatrixType.
  void
  setB(const JacobianMatrixType &newB = JacobianMatrixType{})
  {
    this->B = newB;
    this->firstTime = false;
  }
  void
  setNonLinSys(NonLinearSystemType const *s) override
  {
    JacobianBase::setNonLinSys(s);
    firstTime = true;
  };
  //!
  //! Solves \f$B(x) d = b\f$
  //! \param x The point to compute residual
  //! \param b The right hand side
  //! \return The result of the application of the inverse Jacobian to the
  //! residual
  ArgumentType solve(ArgumentType const &x,
                     ArgumentType const &b) const override;

private:
  //! The Jacobian Matrix
  //!
  //! It is marked mutable since I decided to set solve() as a
  //! const methods, but this entry may be changed by solve!
  mutable JacobianMatrixType B;
  //! solution computed at the previous step
  //! It is marked mutable since I decided to set solve() as a
  //! const methods, but this entry is changed by solve!
  mutable ArgumentType previousX;
  //! value of the function at the previous step
  //! It is marked mutable since I decided to set solve() as a
  //! const methods, but this entry is changed by solve!
  mutable ArgumentType previousR;
  //! used to identify the first time solve() is called
  //! It is marked mutable since I decided to set solve() as a
  //! const methods, but this entry is changed by solve!
  mutable bool firstTime = true;
};
//! Implements Broyden (good) method
//!
//! The so called Broyden method is a quasi-Newton method
//! that uses a formula for updating an approximation of the inverse of the
//! Jacobian. The formula makes use of the last two values of the solution and
//! of the value of the function so that the computed approximation converges to
//! the actual Jacobian
//!
class BroydenG : public JacobianBase
{
public:
  //! If no initial matrix is give, we take the identity.
  //!
  //! Since I do not know at this stage the dimension of the
  //! system, the actual computation of the identity matrix is done
  //! the first time solve() is called.
  using JacobianBase::JacobianBase;
  //!
  //! I have to give an inital matrix, approximation of the Jacobian
  //!
  //! \param B the initial approximation of the Jacobian
  BroydenG(JacobianMatrixType const &B) : B{B}, firstTime{true} {}
  //!
  //! Sets the inital matrix, approximation of the Jacobian
  //!
  //! It can be used also for restarting purposes. When given the matrix
  //! the method restarts from the new iteration given in the method solve()
  //!
  //! \param newB the initial approximation of the Jacobian
  void
  setB(const JacobianMatrixType &newB = JacobianMatrixType{})
  {
    this->B = newB;
    this->firstTime = false;
  }

  void
  setNonLinSys(NonLinearSystemType const *s) override
  {
    JacobianBase::setNonLinSys(s);
    firstTime = true;
  };
  //!
  //! Solves \f$B(x) d = b\f$
  //! \param x The point to compute residual
  //! \param b The right hand side
  //! \return The result of the application of the inverse Jacobian to the
  //! residual
  ArgumentType solve(ArgumentType const &x,
                     ArgumentType const &b) const override;

private:
  //! The Jacobian Matrix
  //!
  //! It is marked mutable since I decided to set solve() as a
  //! const methods, but this entry may be changed by solve!
  mutable JacobianMatrixType B;
  //! solution computed at the previous step
  //! It is marked mutable since I decided to set solve() as a
  //! const methods, but this entry is changed by solve!
  mutable ArgumentType previousX;
  //! value of the function at the previous step
  //! It is marked mutable since I decided to set solve() as a
  //! const methods, but this entry is changed by solve!
  mutable ArgumentType previousR;
  //! used to identify the first time solve() is called
  //! It is marked mutable since I decided to set solve() as a
  //! const methods, but this entry is changed by solve!
  mutable bool firstTime = true;
};
//! Implements Eirola_Nevanlinna method
//!
//! A quasi-Newton method
//! that uses a formula for updating an approximation of the inverse of the
//! Jacobian. The formula makes use of the last two values of the solution and
//! of the value of the function so that the computed approximation "converges"
//! to the actual Jacobian
//!
class Eirola_Nevanlinna : public JacobianBase
{
public:
  //! If no initial matrix is give, we take the identity.
  //!
  //! Since I do not know at this stage the dimension of the
  //! system, the actual computation of the identity matrix is done
  //! the first time solve() is called.
  using JacobianBase::JacobianBase;
  //!
  //! I have to give an inital matrix, approximation of the Jacobian
  //!
  //! \param B the initial approximation of the Jacobian
  Eirola_Nevanlinna(JacobianMatrixType const &B) : B{B}, firstTime{true} {}
  //!
  //! Sets the inital matrix, approximation of the Jacobian
  //!
  //! It can be used also for restarting purposes. When given the matrix
  //! the method restarts from the new iteration given in the method solve()
  //!
  //! \param newB the initial approximation of the Jacobian
  void
  setB(const JacobianMatrixType &newB = JacobianMatrixType{})
  {
    this->B = newB;
    this->firstTime = false;
  }

  void
  setNonLinSys(NonLinearSystemType const *s) override
  {
    JacobianBase::setNonLinSys(s);
    firstTime = true;
  };
  //!
  //! Solves \f$B(x) d = b\f$
  //! \param x The point to compute residual
  //! \param b The right hand side
  //! \return The result of the application of the inverse Jacobian to the
  //! residual
  ArgumentType solve(ArgumentType const &x,
                     ArgumentType const &b) const override;

private:
  //! The Jacobian Matrix
  //!
  //! It is marked mutable since I decided to set solve() as a
  //! const methods, but this entry may be changed by solve!
  mutable JacobianMatrixType B;
  //! solution computed at the previous step
  //! It is marked mutable since I decided to set solve() as a
  //! const methods, but this entry is changed by solve!
  mutable ArgumentType previousX;
  //! value of the function at the previous step
  //! It is marked mutable since I decided to set solve() as a
  //! const methods, but this entry is changed by solve!
  mutable ArgumentType previousR;
  //! used to identify the first time solve() is called
  //! It is marked mutable since I decided to set solve() as a
  //! const methods, but this entry is changed by solve!
  mutable bool firstTime = true;
};

} // namespace apsc
/*
Copyright 2020 <Luca Formaggia, Carlo de Falco>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#endif /* NONLINSYSSOLVER_JACOBIAN_HPP_ */
