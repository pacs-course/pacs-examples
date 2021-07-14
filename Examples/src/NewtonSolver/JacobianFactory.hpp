/*
 * \file JacobianFactory.hpp
 *
 *  Created on: Feb 26, 2020
 *      Author: forma
 *
 *      A little utility to create Jacobians
 */

#ifndef NEWTONSOLVER_JACOBIANFACTORY_HPP_
#define NEWTONSOLVER_JACOBIANFACTORY_HPP_
#include "Jacobian.hpp"
#include <memory>
#include <stdexcept>
namespace apsc
{
  //! Enumerator for different Jacobias
  //!
  //! I use a simple enumerator because it is convertible to an int
  //! I explicitely indicate the numbering, only to facilitate the identification
  //! of the integer corresponding to thee enum (if I want to read it from a file, for instance)
  //!
  enum JacobianKind {DISCRETEJACOBIAN=0, IDENTITYJACOBIAN=1,
    FULLJACOBIAN=2, BROYDENB=3, BROYDENG=4, EIROLANEVANLINNA=5};

  //! A simple factory that returns a JacobianBase polymorphic object wrapped in a unique_prt
  //!
  //! /param kind The kind of Jacobian type class you want
  //! /param args Optional arguments to be forwarded to the constructor
  //!
  template <typename ...Args>
  std::unique_ptr<apsc::JacobianBase> make_Jacobian(JacobianKind kind)
  {
    switch(kind)
    {
      case DISCRETEJACOBIAN:
        return std::make_unique<apsc::DiscreteJacobian>();
        break;
      case IDENTITYJACOBIAN:
         return std::make_unique<apsc::IdentityJacobian>();
         break;
       case FULLJACOBIAN:
         return std::make_unique<apsc::FullJacobian>();
         break;
       case BROYDENB:
         return std::make_unique<apsc::BroydenB>();
         break;
       case BROYDENG:
         return std::make_unique<apsc::BroydenG>();
         break;
       case EIROLANEVANLINNA:
         return std::make_unique<apsc::Eirola_Nevanlinna>();
         break;
     default:
         throw std::runtime_error("Error in JacobianKind: You must specify a valid type");
    }
  }
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



#endif /* NEWTONSOLVER_JACOBIANFACTORY_HPP_ */
