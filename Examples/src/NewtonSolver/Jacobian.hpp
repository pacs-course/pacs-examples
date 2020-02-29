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
    //! The base class for the Jacobian.
    //! It inherits from NonLinSolveTraits to get all types defined there
    //!
    //! It implements the basic methods for the application of the Jacobian, which in fact
    //! may be a "pseudoJacobian" for quasi-Newton methods or evne Picard iterations
    //!
    class JacobianBase: public NewtonTraits
    {
      public:
      //! Takes a pointer to an existing non linear system
      JacobianBase(NonLinearSystemType const * nls=nullptr):M_sys{nls}{};
      //! Solves the system \f$J^{-1}(x) b\f$
      //! /param x the point where to evaluate the Jacobian
      //! /param b the right hand side of the system J(x) y=b
      //! /return the result
      virtual ArgumentType solve(ArgumentType const & x, ArgumentType const & b) const=0;
      //! Sets the non linear system.
      void setNonLinSys(NonLinearSystemType const * s){M_sys=s;};
      virtual ~JacobianBase(){};
      protected:
      //! The non-linear system
      /*!
       * Since it is a small object I just use composition
       */
      NonLinearSystemType const *  M_sys;
      };

    //! Computes the jacobian by finite differences.
     /*!
       The final keyword has been introduced in C++11 to stop
       a hierarchy. No class can be derived from it.

       @note This implementation is mildly dangerous, since it stores a pointer
       to the linear system and nobody ensures that the linear system will not
       be destroyed before the call method of this class is used! But we may consider
       it a very unlikely event.
      */
     class DiscreteJacobian final: public JacobianBase
     {
     public:
       //! Constructor optionally takes the pointer to a non linear system.
       DiscreteJacobian(NonLinearSystemType * sys=nullptr, double spacing=defaultTol):JacobianBase{sys},
                                                M_tol{spacing}{};
      //! Sets the spacing for computing finite differences
       void setTol(double tol){M_tol=tol;}
       //! Returns the tolerance currently used.
       auto getTol()const {return M_tol;}
       //! Solves the system
       //!
       //! Computes the Jacobian using finite differences.
       //! /param x the point where to evaluate the Jacobian
       //! /param b the right hand side of the system J(x) y=b
       //! /return the result
        ArgumentType solve(ArgumentType const & x, ArgumentType const & b) const override;
      double
      getLambda () const
      {
        return lambda;
      }

      void setLambda(double lambda)
      {
        this->lambda = lambda;
      }

     private:
       //! The current spacing
       double M_tol;
       //! Regularization (if needed)
       double lambda{0.0};
       //! Default tolerance.
       static double constexpr defaultTol=1e-4;
     };
  //! The identityJacobian just scales the residual by a factor defaulted to 1
     class IdentityJacobian final: public JacobianBase
     {
     public:
       //! inherit base constructor
       using JacobianBase::JacobianBase;
       //! Returns the rhs b scaled by lambda
       //! /param x (not used)
       //! /param b the right hand side of the system \f$\alpha^{-1} I y=b\f$
       //! /return the result

       ArgumentType solve(ArgumentType const & x, ArgumentType const & b) const override
       {
         return this->lambda*b;
       }
      //! get the scaling factor
      double
      getLambda () const
      {
        return lambda;
      }
      //! Set the scaling factor
      void
      setLambda (double lambda = 1.0)
      {
        this->lambda = lambda;
      }

     private:
       double lambda=1.0;
     };

     //! Here I explicitly set the Jacobian as a function
  class FullJacobian: public JacobianBase
  {
  public:
       //! Inherit base constructor
       using JacobianBase::JacobianBase;
       FullJacobian (FullJacobian const &)=default;
       FullJacobian (FullJacobian &&)=default;
       FullJacobian & operator=(FullJacobian const &)=default;
       FullJacobian & operator=(FullJacobian&&)=default;

       //! I can pass the JacobianFunction via constructor
       //! \param j an object convertible to a JacobianFunction
       FullJacobian(JacobianFunctionType const & j,NonLinearSystemType * nls=nullptr):JacobianBase{nls},Jac{j}{};
       //! I can change the JacobianFunction
       //! \tparam J a type convertible to JacobianFunction
       //! \param j an object convertible to a JacobianFunction
       template <class J>
       void setJacobianFunction(J&& j){Jac=std::forward<J>(j);}
       //! I may vet a copy of the jacobianFunction
       //! \return A copy of the sored JacobianFunction
       JacobianFunctionType getJacobianFunction() const {return Jac;}
       //! Solve \f$J(x) d = - b\f$
       //! \param x The point to compute residual
       //! \param b The right hand side
       //! \return The result of the application of the inverse Jacobian to the residual
       ArgumentType solve(ArgumentType const & x, ArgumentType const & b) const override;
  private:
       JacobianFunctionType Jac;
  };

  //! Implements Broyden (bad) method
  //!
  //! The so called Broyden bad method (which is not so bad after all) is a quasi-Newton method
  //! that uses a formula for updating an approximation of the inverse of the Jacobian.
  //! The formula makes use of the last two values of the solution and of the value of the function
  //! so that the computed approximation converges to the actual Jacobian
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
    BroydenB (JacobianMatrixType const & B) : B{B}{ }
    //!
    //! Sets the inital matrix, approximation of the Jacobian
    //!
    //! It can be used also for restarting purposes. When given the matrix
    //! the method restarts from the new iteration given in the method solve()
    //!
    //! \param B the initial approximation of the Jacobian
    void setB (const JacobianMatrixType& previousB)
    {
      this->B = previousB;
      this->firstTime=true;
    }
    //!
    //! Solves \f$B(x) d = b\f$
    //! \param x The point to compute residual
    //! \param b The right hand side
    //! \return The result of the application of the inverse Jacobian to the residual
    ArgumentType solve(ArgumentType const & x, ArgumentType const & b) const override;
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
     mutable bool firstTime=true;
  };
  //! Implements Broyden (good) method
   //!
   //! The so called Broyden method is a quasi-Newton method
   //! that uses a formula for updating an approximation of the inverse of the Jacobian.
   //! The formula makes use of the last two values of the solution and of the value of the function
   //! so that the computed approximation converges to the actual Jacobian
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
     BroydenG (JacobianMatrixType const & B) : B{B}{ }
     //!
     //! Sets the inital matrix, approximation of the Jacobian
     //!
     //! It can be used also for restarting purposes. When given the matrix
     //! the method restarts from the new iteration given in the method solve()
     //!
     //! \param B the initial approximation of the Jacobian
     void setB (const JacobianMatrixType& previousB)
     {
       this->B = previousB;
       this->firstTime=true;
     }
     //!
     //! Solves \f$B(x) d = b\f$
     //! \param x The point to compute residual
     //! \param b The right hand side
     //! \return The result of the application of the inverse Jacobian to the residual
     ArgumentType solve(ArgumentType const & x, ArgumentType const & b) const override;
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
      mutable bool firstTime=true;
   };
 }
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
