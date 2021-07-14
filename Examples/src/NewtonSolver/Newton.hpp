/*
 * Newton.hpp
 *
 *  Created on: Feb 21, 2020
 *      Author: forma
 */

#ifndef NONLINSYSSOLVER_NEWTON_HPP_
#define NONLINSYSSOLVER_NEWTON_HPP_
#include <memory>
#include <limits>
#include "NewtonMethodsSupport.hpp"
#include "Jacobian.hpp"

namespace apsc
{

  //! This class implements Newton method
  //!
  //! It gets the definition of the main types from NonLinSolverTraits. This is a way to make sure that also
  //! the Jacobian uses the same types, since it inherits form NonLinSolverTraits as well.
  //!
  //! I have chosen a particular construction. The non linear system to be solved is composed into the class as private member,
  //! while I have a unique_ptr to the Jacobian base class. In this way also the jacobian is composed into the class. However,
  //! As a result the class is not copy-constructible not copy-assignable. To have copy operators that perform deep copy I shoul
  //! implement a clone() method in the Jacobian classes.
  //!
  class Newton: public apsc::NewtonTraits
  {
  public:
    Newton()=default;
    //! I can pass options and jacobian. This is usefule for classes that store a Newton object
    Newton(std::unique_ptr<JacobianBase> j,NewtonOptions opt=NewtonOptions{}): Jacobian_ptr{std::move(j)},
        options{opt} {};
    //!This class not meant to be copy constructible
    //! The implementation of copy constructor would require a mechanism
    //! for the deep copy of the Jacobian, which is passed as a unique pointer
    Newton(Newton const &)=delete;
    //!This class not meant to be copy assignable.
    Newton & operator=(Newton const &)=delete;
    //! It can be moved constructed
    Newton(Newton&& n)=default;
    //! It can be move assigned
    Newton & operator=(Newton&&)=default;
    //! This constructor accepts a non linear system and a unique pointer to the Jacobian base class
    template<class NLS>
    Newton(NLS&& nls, std::unique_ptr<JacobianBase> j,NewtonOptions opt=NewtonOptions{}):
    nonLinSys(std::forward<NLS>(nls)),Jacobian_ptr(std::move(j)),options(opt)
    {
      // I need to connect the Jacobian to the linear system
      Jacobian_ptr->setNonLinSys(&nonLinSys);
    }
    //! This constructor accepts a non linear system and a concrete Jacobian object
    template<class NLS, class JAC>
    Newton(NLS&& nls, JAC const j,NewtonOptions opt=NewtonOptions{}):
    nonLinSys(std::forward<NLS>(nls)),Jacobian_ptr(std::make_unique<JAC>(j)),
    options(opt)
        {
      // I need to connect the Jacobian to the non linear system
      Jacobian_ptr->setNonLinSys(&nonLinSys);
        }

    //! You can change the Jacobian. Note that it is moved
    //! You have to explicitly move if it is not a rvalue!
    //! For instance:
    //! \code
    //!   myNewton.setJacobianPtr(std::move(passedPointer));
    //! \endcode
    void
    setJacobianPtr (std::unique_ptr<JacobianBase> jacobianPtr)
    {
      Jacobian_ptr=std::move(jacobianPtr);
      // Connect Jacobian to the non-linear system
      Jacobian_ptr->setNonLinSys(&nonLinSys);
      // Reset the state so I can reuse the object
      this->state=NewtonState{};
    }
    //! You can get the options
    const NewtonOptions&
    getOptions () const
    {
      return options;
    }
    //! You can set options
    void
    setOptions (const NewtonOptions& options)
    {
      this->options = options;
      this->state=NewtonState{};

    }

    //! Get a copy of the non linear system
    NonLinearSystemType
    getNonLinSys () const
    {
      return nonLinSys;
    }
    //! Update the non linear system
    template<class NLS>
    void
    setNonLinSys (NLS&& nonLinSystem)
    {
      this->nonLinSys = std::forward<NLS>(nonLinSystem);
      this->state=NewtonState{};
      Jacobian_ptr->setNonLinSys(&nonLinSys);
    }

    apsc::NewtonResult solve(ArgumentType const & x0);
    virtual ~Newton()=default;
  protected:
    virtual void callback()const {};
    //! Internal structure giving the state
    //! Used for callback
    struct NewtonState
    {
      //! The last computed solution
      NewtonTraits::ReturnType currentSolution;
      //! The current number of iterations
      unsigned int currentIteration{0u};
      //! The current residualNorm (set initially to a big number)
      double currentResidualNorm=std::numeric_limits<double>::max();
      //! current step length (set initially to a big number)
      double currentStepLength=std::numeric_limits<double>::max();
        };

    NonLinearSystemType nonLinSys;
    std::unique_ptr<JacobianBase> Jacobian_ptr;
    NewtonOptions options;
    NewtonState state;
  };

  //! An example of  use of callback()
  //!
  //! A verbose version of Newton
  //!
  struct NewtonVerbose: public Newton
  {
    //! Inherit constructors
    using Newton::Newton;
    //! Specialization of callback()
    void callback() const override;
  };
};



#endif /* NONLINSYSSOLVER_NEWTON_HPP_ */
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
