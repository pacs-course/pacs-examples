/*
 * Newton.cpp
 *
 *  Created on: Feb 21, 2020
 *      Author: forma
 */

#include <exception>
#include <iostream>
#include "Newton.hpp"
apsc::NewtonResult
apsc::Newton::solve (const ArgumentType& x0)
{
  using namespace apsc;
  // C++17 structured bindings
  // Get all options
  //  const auto & [tolerance, minRes, maxIter, backTrackOn, stopOnBacktracking,
  //                alpha, backStepReduction, maxBackSteps, lambdaInit] = this->options;
  // get all variables that go in the state
   // This is needed to allow an overridden callback to access the state

  //  auto & [currentSolution, currentIteration,
  //          currentResidualNorm,currentStepLength] = this->state;

  // Get all options
  const auto & tolerance = this->options.tolerance;
  const auto & minRes    = this->options.minRes;
  const auto & maxIter   = this->options.maxIter;
  const auto & backtrackOn = this->options.backtrackOn;
  const auto & stopOnStagnation = this->options.stopOnStagnation;
  const auto & alpha       = this->options.alpha;
  const auto & backStepReduction = this->options.backstepReduction;
  const auto & maxBackSteps = this->options.maxBackSteps;
  const auto & lambdaInit = this->options.lambdaInit;

  // get all variables that go in the state
  // This is needed to allow an overridden callback to access the state
  auto & currentSolution = this->state.currentSolution;
  auto & currentIteration = this->state.currentIteration;
  auto & currentResidualNorm=this->state.currentResidualNorm;
  auto & currentStepLength = this->state.currentStepLength;
  // I make sure that the Jacobian has the correct non linear system
  // This is important if the Newton object has been moved
  Jacobian_ptr->setNonLinSys(&nonLinSys);
  // The initial step is to compute the relevant quantities
  // from the initial conditions
  currentSolution = x0;
  auto residual = this->nonLinSys(currentSolution);
  // Test if we have a map Rn to Rn
  if (currentSolution.size()!=residual.size())
    throw std::runtime_error("Newton needs a function Rn to Rn");
  currentResidualNorm = residual.norm();
  // initial values for tests of convergence and stagnation
  bool converged  =  false;
  bool stagnation =  false;
  bool stop       =  false;
  bool previousNoDecrease=false;

// I always do at least 1 step. I might eventually check if the residual is already small
  do
    {
      ++currentIteration;
      auto previousSolution=currentSolution;
      auto previousResidualNorm=currentResidualNorm;
      double lambda = lambdaInit; // initial step length. For Newton type algorithms is always 1
      // compute the delta
      ArgumentType delta  = this->Jacobian_ptr->solve(currentSolution,residual);
      currentStepLength   = delta.norm();
      currentSolution     = previousSolution - lambda*delta; // update solution
      residual            = this->nonLinSys(currentSolution);
      currentResidualNorm = residual.norm(); // recompute residual norm
      if(backtrackOn) // activate backtracking
        {
          auto backIter=0u;
          while((backIter < maxBackSteps) && (currentResidualNorm>(1.0-lambda*alpha)*previousResidualNorm))
            {
              ++backIter;
              lambda  *=  backStepReduction; // reduce advance
              currentSolution = previousSolution - lambda*delta; // recompute solution
              residual        = this->nonLinSys(currentSolution); // compute new residual
              currentResidualNorm = residual.norm(); // recompute residual norm
            };
        }
      // If residual does not descent for two consecutive iterations we are stagnating
      bool noDecrease    = (currentResidualNorm>=previousResidualNorm);
      stagnation         = previousNoDecrease && noDecrease;
      previousNoDecrease = noDecrease;
      // We stop on stagnation if we have decided to
      stop=stagnation && stopOnStagnation;
      // test convergence
      converged = (currentResidualNorm <= minRes) && (currentStepLength <= tolerance);
      this->callback(); // This allows a specialization of callback to operate
    }
  while ((!converged) && (currentIteration < maxIter) && (!stop));

  return NewtonResult{currentSolution,currentResidualNorm,currentStepLength,currentIteration,converged,stagnation};

}

void
apsc::NewtonVerbose::callback () const
{
  std::cout<<"Iteration "<< this->state.currentIteration;
  std::cout<<" Residual "<<this->state.currentResidualNorm;
  std::cout<<" Step length "<<this->state.currentStepLength;
  std::cout<<std::endl;
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
