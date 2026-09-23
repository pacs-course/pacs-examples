#include "Newton_ad.hpp"
#include <autodiff/forward/real/eigen.hpp>
#include <iostream>
#include <stdexcept>

namespace
{
/*!
 * \brief Extract the values from a vector of autodiff::Real numbers.
 * \param x The vector of autodiff::Real numbers.
 * \return A vector of double values.
 */
Eigen::VectorXd
values(const autodiff::VectorXreal &x)
{
  Eigen::VectorXd result(x.size());
  for(Eigen::Index i = 0; i < x.size(); ++i)
    result(i) = autodiff::val(x(i));
  return result;
}
} // namespace

apsc::newton_ad::NewtonResult
apsc::newton_ad::Newton::solve(const ArgumentType &x0)
{
  if(!nonLinSys)
    throw std::runtime_error("ERROR: Non linear system not initialized");

  auto &[currentSolution, currentIteration, currentResidualNorm,
         currentStepLength] = state;
  currentSolution = x0;
  currentIteration = 0u;
  currentStepLength = std::numeric_limits<double>::max();

  auto evaluate = [this](const ArgumentType &x, JacobianMatrixType &jacobian) {
    AutoDiffArgumentType xad = x; // here is where we convert the standard Eigen
                                  // vector to autodiff::VectorXreal
    AutoDiffReturnType residual;
    autodiff::jacobian(nonLinSys, autodiff::wrt(xad), autodiff::at(xad),
                       residual, jacobian);
    return values(residual);
  };

  JacobianMatrixType jacobian;
  auto               residual = evaluate(currentSolution, jacobian);
  if(currentSolution.size() != residual.size())
    throw std::runtime_error("ERROR: Newton needs a function Rn to Rn");

  currentResidualNorm = residual.norm();
  bool converged = false;
  bool stagnation = false;
  bool stop = false;
  bool previousNoDecrease = false;
  if(currentResidualNorm <= options.minRes)
    return NewtonResult{
      currentSolution, currentResidualNorm, 0.0, currentIteration, true, false};

  do
    {
      ++currentIteration;
      const auto         previousSolution = currentSolution;
      const double       previousResidualNorm = currentResidualNorm;
      double             lambda = options.lambdaInit;
      const ArgumentType delta = jacobian.fullPivLu().solve(residual);
      currentStepLength = delta.norm();
      currentSolution = previousSolution - lambda * delta;
      residual = evaluate(currentSolution, jacobian);
      currentResidualNorm = residual.norm();

      if(options.backtrackOn)
        for(unsigned int backIter = 0;
            backIter < options.maxBackSteps &&
            currentResidualNorm >
              (1.0 - lambda * options.alpha) * previousResidualNorm;
            ++backIter)
          {
            lambda *= options.backstepReduction;
            currentSolution = previousSolution - lambda * delta;
            residual = evaluate(currentSolution, jacobian);
            currentResidualNorm = residual.norm();
          }

      const bool noDecrease = currentResidualNorm >= previousResidualNorm;
      stagnation = previousNoDecrease && noDecrease;
      previousNoDecrease = noDecrease;
      stop = stagnation && options.stopOnStagnation;
      converged = currentResidualNorm <= options.minRes &&
                  currentStepLength <= options.tolerance;
      callback();
  } while(!converged && currentIteration < options.maxIter && !stop);

  return NewtonResult{currentSolution,  currentResidualNorm, currentStepLength,
                      currentIteration, converged,           stagnation};
}

void
apsc::newton_ad::NewtonVerbose::callback() const
{
  std::cout << "Iteration " << state.currentIteration << " Residual "
            << state.currentResidualNorm << " Step length "
            << state.currentStepLength << '\n';
}
