/*!
 * @file
 * @brief Class for NewtonTraits
 *
 * This class is a template class used to define the types used by the Newton
 * solver It is used to simplify the implementation of the Newton solver It is a
 * template class to allow the user to select different types for the non-linear
 * system and for the Jacobian matrix
 * @author Luca Formaggia
 * @date 19 Feb 2020
 */
/*
 * NewtonTraits.hpp
 *
 * NetwtonTraits.hpp
 *
 *  Created on: Feb 19, 2020
 *      Author: forma
 *  @file
 */
#ifndef NONLINSYSSOLVER_NEWTONTRAITS_HPP_
#define NONLINSYSSOLVER_NEWTONTRAITS_HPP_
#include <Eigen/Dense>
#include <functional>
namespace apsc
{
/*!
@brief A class to define the types used by the Newton solver

Here I base the class on Eigen matrices and vectors. This is a choice, but
it is a good one, since Eigen is a very efficient library for linear algebra,

However, by changing the trait and add some adaptors I can do as in
the FixedPoint class and use standard vectors or other types of containers.
*/
struct NewtonTraits
{
  //! The type of argument: an Eigen dynamic vector
  using ArgumentType = Eigen::Matrix<double, Eigen::Dynamic, 1>;
  //! The return type: we want a function \f$R^N\to R^N\f$
  using ReturnType = ArgumentType;
  //! The type for the non-linear system
  using NonLinearSystemType = std::function<ReturnType(const ArgumentType &)>;
  //! Type used to store the Jacobian as matrix
  using JacobianMatrixType = Eigen::MatrixXd;
  //! Type for the JacobianFunction
  using JacobianFunctionType =
    std::function<JacobianMatrixType(const ArgumentType &)>;
};
} // end namespace apsc

#endif /* NONLINSYSSOLVER_NEWTONTRAITS_HPP_ */
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
