/*
 * NewtonMethodsSupport.hpp
 *
 *  Created on: Feb 2, 2020
 *      Author: forma
 */

#ifndef NONLINSYSSOLVER_NEWTONMETHODSSUPPORT_HPP_
#define NONLINSYSSOLVER_NEWTONMETHODSSUPPORT_HPP_
#include "Eigen/Dense"

#include "NewtonTraits.hpp"
namespace apsc
{

   //! The options to be passed, with default values
   //!
   //! \note
   //! We use absolute tolerances in the code. It is the user task to set them appropriately
   //! \todo
   //! Implement relative tolerances
   struct NewtonOptions{
   //! @brief Tolerance for two successive iterations.
   /*! @detail The iteration stops if \f$||x_{new}-x_{old}||<tolerance\f$. */
   double tolerance{1.e-8};
   //! @brief Tolerance on residual.
   /*! @detail The iteration stops if \f$||F(x)||<minRes\f$.*/
   double minRes{1.e-6};
   //! Max. number of iterations (not including possible backtracking steps).
   unsigned int maxIter{50};
   //! If we want backtracking
   bool backtrackOn{false};
   //! Stop if we stagnate.
   //! We stagnate if for two consecutive iterations the residual does not decrease.
   bool stopOnStagnation{false};
   //! Sufficient descent parameter (typically 10^-4) for backtracking
   //!
   //! This parameter is used to test sufficient decrease condition (also called 1st Wolfe condition)
   double alpha{1e-4};
   //! Backtracking reduction (a value in (0,1), typically 0.5);
   double backstepReduction{0.5};
   //! Max number of backtracking iterations (4 is typical)
   unsigned int maxBackSteps{4};
   //! Initial alpha
   //! This is the initial value of the parameter alpha in the iteration
   //! \f[
   //!  x_{k+1}=x_k + \lambda \delta x
   //! \f]
   //! where \f$\delta x\f$ is the increment computed by the solution of \f$ J\delta x =-r\f$
   //!
   //! \note
   //! For Newton and Broyden it is essential for achieving the optimal convergence that \f$\lambda =1\f$
   //! is chosen. So there must be an integer N so that
   //! \code
   //! lambdamax*N*backstepReduction=1
   //! \endcode
   double lambdaInit=1.0;
 };

//! The quantities returned, wrapped in a struct
  struct NewtonResult{
  //! The solution
    NewtonTraits::ReturnType solution;
    //! Last norm of residual.
    double residualNorm{0.0};
    //! The last step length
    double stepLength{0.0};
    //! The last value of the iteration counter
    unsigned int iterations{0u};
    //! \brief Convergence flag. True if convergence is reached
    //! \detail Convergence is reached when both conditions on residual and step length
    //! are met.
    bool converged=false;
    //! Stagnation flag. True if we have stagnated.
    bool stagnation{false};
  };

}

#endif /* NONLINSYSSOLVER_NEWTONMETHODSSUPPORT_HPP_ */
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
