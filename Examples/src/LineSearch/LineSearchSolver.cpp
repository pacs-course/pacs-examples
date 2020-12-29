/*
 * LineSearchSolver.cpp
 *
 *  Created on: Dec 28, 2020
 *      Author: forma
 */
#include "LineSearchSolver.hpp"
#include <cmath>
#include <iostream>
#include <exception>
void
apsc::LinearSearchSolver::setInitialPoint(
  apsc::LineSearch_traits::Vector initialPoint)
{
  if(!this->optimizationData.costFunction || !this->optimizationData.gradient)
    {
      throw std::runtime_error("You cannot set an initial point before setting cost function and gradient\n");
    }
  this->descentDirectionFinderPtr->reset(); // reset the descent direction computation (when needed)
  this->currentValues.currentPoint=initialPoint;
  this->currentValues.currentGradient=this->optimizationData.gradient(initialPoint);
  this->currentValues.currentCostValue=this->optimizationData.costFunction(initialPoint);
  std::size_t n=initialPoint.size();
  if (n != this->optimizationData.NumberOfVariables)
    {
      std::cerr<<"Number of variables indicated in the data ("<<this->optimizationData.NumberOfVariables<<
          ") does not correspond to the size of the initial point("<<n<<").\n  I am fixing it, hoping for the best.\n";
      this->optimizationData.NumberOfVariables=n;
    }
}

std::tuple<apsc::OptimizationCurrentValues, std::size_t, int>
apsc::LinearSearchSolver::solve()
{
  auto const & relTol = this->options.relTol;
  auto const & absTol = this->options.absTol;
  auto const & maxIter = this->options.maxIter;
  auto & currentPoint =this->currentValues.currentPoint;
  auto & currentValue =this->currentValues.currentCostValue;
  auto & currentGradient = this->currentValues.currentGradient;
  auto gradientNorm=currentGradient.norm();
  // the relative tolerance is respect initial gradient norm.
  auto const testValue=relTol*gradientNorm;
  std::size_t  iter=0;


  while (gradientNorm>(testValue+absTol) and iter<maxIter)
    {
      // get descent direction
      apsc::LineSearch_traits::Vector dd = this->descentDirectionFinderPtr->operator()(this->currentValues);
      // backtrack
      auto [newPoint,newValue,status] = this->backtrack(dd);
      // check what happened
      if (status!=0)
        {
          if (status==1)
            throw std::runtime_error("I have found a non-descent direction. Something wrong. This code does not allow it!\n");
          else
            throw std::runtime_error("I cannot satisfy the sufficient decrease condition. Try to change line search options\n");
        }
      currentPoint=newPoint;
      currentValue=newValue;
      currentGradient=this->optimizationData.gradient(newPoint);
      gradientNorm=currentGradient.norm();
      ++iter;
    }
  int status= iter<maxIter?0:1;
  return {this->currentValues,iter,status};
}

// A simple backtracking. I implement only sufficient decrease condition.
std::tuple<apsc::LineSearch_traits::Vector, apsc::LineSearch_traits::Scalar,
           int>
apsc::LinearSearchSolver::backtrack(
  const apsc::LineSearch_traits::Vector &searchDirection) const
{
  apsc::OptimizationData const & data=this->optimizationData;
  apsc::OptimizationCurrentValues const & currentValues=this->currentValues;
  apsc::LineSearchOptions const & options=this->lineSearchOptions;
  using Scalar= apsc::LineSearch_traits::Scalar;
  using Vector= apsc::LineSearch_traits::Vector;
  using CostFunction=apsc::LineSearch_traits::CostFunction;

  Scalar gradstep = currentValues.currentGradient.transpose()*searchDirection;
  // Check is direction is descent direction
  if (gradstep >=0.)
    {
      // Bad direction: return with bad status.
      return {currentValues.currentPoint, currentValues.currentCostValue,1};
    }

  CostFunction const & f=data.costFunction;
  Vector currentPoint = currentValues.currentPoint;
  auto const & maxIter=options.maxIter;
  auto alpha=options.initialStep;
  unsigned int iter=0u;
  apsc::LineSearch_traits::Vector nextPoint;
  nextPoint=currentPoint + alpha*searchDirection;
  Scalar nextValue=f(nextPoint);
  // iterate until sufficient decrease condition is met.
  while ((nextValue>currentValues.currentCostValue +options.sufficientDecreaseCoefficient*alpha*gradstep) and (iter<maxIter))
    {
      ++iter;
      alpha*=options.stepSizeDecrementFactor;
      nextPoint=currentPoint + alpha*searchDirection;
      nextValue=f(nextPoint);
    }
  int status=iter<maxIter?0:2;
  return {nextPoint, nextValue, status};
}
