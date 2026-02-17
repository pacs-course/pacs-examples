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
//! Enumerator for different Jacobians
//!
//! I use a simple enumerator because it is convertible to an int
//! I explicitly indicate the numbering, only to facilitate the identification
//! of the integer corresponding to three enum (if I want to read it from a
//! file, for instance)
//!
enum class JacobianKind : int
{
  DiscreteJacobian = 0,
  IdentityJacobian = 1,
  FullJacobian = 2,
  BroydenB = 3,
  BroydenG = 4,
  EirolaNevanlinna = 5
};

//! A simple factory that returns a JacobianBase polymorphic object wrapped in a
//! unique_ptr
//!
//! \param kind The kind of Jacobian type class you want
//! \param args Optional arguments to be forwarded to the constructor
//!
template <typename... Args>
[[nodiscard]] std::unique_ptr<apsc::JacobianBase>
make_Jacobian(JacobianKind kind, Args &&...args)
{
  switch(kind)
    {
    case JacobianKind::DiscreteJacobian:
      return std::make_unique<apsc::DiscreteJacobian>(
        std::forward<Args>(args)...);
    case JacobianKind::IdentityJacobian:
      return std::make_unique<apsc::IdentityJacobian>(
        std::forward<Args>(args)...);
    case JacobianKind::FullJacobian:
      return std::make_unique<apsc::FullJacobian>(std::forward<Args>(args)...);
    case JacobianKind::BroydenB:
      return std::make_unique<apsc::BroydenB>(std::forward<Args>(args)...);
    case JacobianKind::BroydenG:
      return std::make_unique<apsc::BroydenG>(std::forward<Args>(args)...);
    case JacobianKind::EirolaNevanlinna:
      return std::make_unique<apsc::Eirola_Nevanlinna>(
        std::forward<Args>(args)...);
    default:
      throw std::runtime_error(
        "Error in JacobianKind: You must specify a valid type");
    }
}
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

#endif /* NEWTONSOLVER_JACOBIANFACTORY_HPP_ */
