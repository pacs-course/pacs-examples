#include "Newton.hpp"

#include <exception>
#include <iostream>

NewtonResult
Newton::solve(const ArgumentType &x0)
{
  // C++17 structured bindings
  // Get all options
  const auto &[tolerance, minRes, maxIter, stopOnStagnation] =
    this->options;

  auto &[currentSolution, currentIteration, currentResidualNorm,
         currentStepLength] = this->state;

  // The initial step is to compute the relevant quantities
  // from the initial conditions
  currentSolution = x0;

  auto residual = this->nonLinSys(currentSolution);

  // Test if we have a map Rn to Rn
  if (currentSolution.size() != residual.size())
    throw std::runtime_error("Newton needs a function from Rn to Rn");

  currentResidualNorm = residual.norm();

  // initial values for tests of convergence and stagnation
  bool converged          = false;
  bool stagnation         = false;
  bool stop               = false;
  bool previousNoDecrease = false;

  // I always do at least 1 step. I might eventually check if the
  // residual is already small
  do
    {
      ++currentIteration;

      auto previousSolution     = currentSolution;
      auto previousResidualNorm = currentResidualNorm;

      // compute the delta
      ArgumentType delta =
        this->Jacobian_ptr->solve(currentSolution, residual);

      currentStepLength = delta.norm();
      currentSolution   = previousSolution - delta;

      residual            = this->nonLinSys(currentSolution);
      currentResidualNorm = residual.norm();

      // If residual does not descent for two consecutive iterations
      // we are stagnating
      bool noDecrease = (currentResidualNorm >= previousResidualNorm);
      stagnation      = (previousNoDecrease && noDecrease);
      previousNoDecrease = noDecrease;

      // We stop on stagnation if we decided to
      stop = (stagnation && stopOnStagnation);

      // Test convergence
      converged = ((currentResidualNorm <= minRes) &&
                   (currentStepLength <= tolerance));

      this->callback();
    }
  while ((!converged) && (currentIteration < maxIter) && (!stop));

  return NewtonResult{currentSolution,
                      currentResidualNorm,
                      currentStepLength,
                      currentIteration,
                      converged,
                      stagnation};
}

void
NewtonVerbose::callback() const
{
  std::cout << "    Iteration " << this->state.currentIteration;
  std::cout << ", residual: " << this->state.currentResidualNorm;
  std::cout << ", step length: " << this->state.currentStepLength;
  std::cout << std::endl;
}
