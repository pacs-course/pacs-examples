/*
 * NetwtonTraits.hpp
 *
 *  Created on: Feb 19, 2020
 *      Author: forma
 *  @file
 */
#ifndef NONLINSYSSOLVER_NEWTONTRAITS_HPP_
#define NONLINSYSSOLVER_NEWTONTRAITS_HPP_
#include <functional>
#include "Eigen/Dense"
namespace apsc
{
  //! We base the Newton solver on Eigen for simplicity
  //! In principle this class should be a template class
  //! to enable choosing different type by selecting a different template argument
  //! during the instantiation of a Newton template class
  //! For simplicity I omit this complication here
  //!
  struct NewtonTraits{
   //! The type of argument: an Eigen dynamic vector
    using ArgumentType=Eigen::Matrix<double,Eigen::Dynamic,1>;
    //! The return type: we want a function \f$R^N\to R^N\f$
    using ReturnType=ArgumentType;
    //! The type for the non-linear system
    using NonLinearSystemType=std::function<ReturnType (const ArgumentType &)>;
    //! Type used to store the Jacobian as matrix
    using JacobianMatrixType= Eigen::MatrixXd;
    //! Type for the JacobianFunction
    using JacobianFunctionType=std::function<JacobianMatrixType (const ArgumentType &)>;
  };
}// end namespace apsc


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
