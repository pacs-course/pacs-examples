/*
 * LineSearchSolver.cpp
 *
 *  Created on: Dec 28, 2020
 *      Author: forma
 */
#include "LineSearchSolver.hpp"
#include <cmath>
#include <exception>
#include <iostream>
void
apsc::LinearSearchSolver::setInitialPoint(
  apsc::LineSearch_traits::Vector initialPoint)
{
  if(!this->optimizationData.costFunction || !this->optimizationData.gradient)
    {
      throw std::runtime_error("You cannot set an initial point before setting "
                               "cost function and gradient\n");
    }
  this->descentDirectionFinderPtr
    ->reset(); // reset the descent direction computation (when needed)
  this->currentValues.currentPoint = initialPoint;
  this->currentValues.currentGradient =
    this->optimizationData.gradient(initialPoint);
  this->currentValues.currentCostValue =
    this->optimizationData.costFunction(initialPoint);
  this->currentValues.currentHessian=this->optimizationData.hessian(initialPoint);
  // This is for bounded problems
  this->currentValues.bounded=this->optimizationData.bounded;
  this->currentValues.lowerBounds=this->optimizationData.lowerBounds;
  this->currentValues.upperBounds=this->optimizationData.upperBounds;

  std::size_t n = initialPoint.size();
  if(n != this->optimizationData.NumberOfVariables)
    {
      std::cerr << "Number of variables indicated in the data ("
                << this->optimizationData.NumberOfVariables
                << ") does not correspond to the size of the initial point("
                << n << ").\n  I am fixing it, hoping for the best.\n";
      this->optimizationData.NumberOfVariables = n;
    }
  if(optimizationData.bounded)
    {
      auto good = true;
      for(auto i = 0u; i < optimizationData.NumberOfVariables; ++i)
        {
          good = good and initialPoint[i] >= optimizationData.lowerBounds[i] and
                 initialPoint[i] <= optimizationData.upperBounds[i];
        }
      if(not good)
        throw std::runtime_error("Initial point must be within given bounds");
    }
}

std::tuple<apsc::OptimizationCurrentValues, std::size_t, int>
apsc::LinearSearchSolver::solve()
{
  auto const &relTol = this->options.relTol;
  auto const &absTol = this->options.absTol;
  auto const &maxIter = this->options.maxIter;
  auto &      currentPoint = this->currentValues.currentPoint;
  auto &      currentValue = this->currentValues.currentCostValue;
  auto &      currentGradient = this->currentValues.currentGradient;
  auto &      currentHessian  = this->currentValues.currentHessian;
  auto        gradientNorm = currentGradient.norm();
  // the relative tolerance is respect initial gradient norm.
  auto const  testValue = relTol * gradientNorm;
  std::size_t iter = 0;
//  bool const &bounded = this->optimizationData.bounded;
  auto        stepLength = 2 * absTol;
  while(gradientNorm > (testValue + absTol) and stepLength > absTol and
        iter < maxIter)
    {
      // get descent direction
      apsc::LineSearch_traits::Vector    dd =
        this->descentDirectionFinderPtr->operator()(this->currentValues);
      auto [newPoint, newValue, status] = this->backtrack(dd);
      // check what happened
      if(status != 0)
        {
          if(status == 1)
            throw std::runtime_error(
              "I have found a non-descent direction. Something wrong. This "
              "code does not allow it!\n");
          else //if(not bounded) // this test is disabled for bounded problems
            throw std::runtime_error(
              "I cannot satisfy the sufficient decrease condition. Try to "
              "change line search options\n");
        }
      stepLength = (newPoint - currentPoint).norm();
      currentPoint = newPoint;
      currentValue = newValue;
      currentGradient = this->optimizationData.gradient(newPoint);
      currentHessian = this->optimizationData.hessian(newPoint);
      gradientNorm = currentGradient.norm();
#ifdef VERBOSE
      std::clog << "iter=" << iter << "\t Grad=" << gradientNorm
                << "\t StepL/Abstol=" << stepLength / absTol << " Point "
                << " " << currentPoint.transpose() << std::endl;
#endif
      ++iter;
    }
  int status = iter < maxIter ? 0 : 1;
  return {this->currentValues, iter, status};
}

// A simple backtracking. I implement only sufficient decrease condition.
std::tuple<apsc::LineSearch_traits::Vector, apsc::LineSearch_traits::Scalar,
           int>
apsc::LinearSearchSolver::backtrack(
  apsc::LineSearch_traits::Vector &searchDirection) const
{
  apsc::OptimizationData const &         data = this->optimizationData;
  apsc::OptimizationCurrentValues const &currentValues = this->currentValues;
  apsc::LineSearchOptions const &        options = this->lineSearchOptions;
  using Scalar = apsc::LineSearch_traits::Scalar;
  using Vector = apsc::LineSearch_traits::Vector;
  using CostFunction = apsc::LineSearch_traits::CostFunction;

  Scalar gradstep = currentValues.currentGradient.transpose() * searchDirection;
  // Check is direction is descent direction
  if(gradstep >= 0.)
    {
      if(searchDirection.norm() == 0.)
        return {currentValues.currentPoint, currentValues.currentCostValue, 0};
      else
        {
          std::cerr << gradstep << " not valid. Reverted to gradient\n";
          searchDirection = -currentValues.currentGradient;
          gradstep = -searchDirection.squaredNorm();
        }
    }

  CostFunction const &            f = data.costFunction;
  Vector                          currentPoint = currentValues.currentPoint;
  auto const &                    maxIter = options.maxIter;
  auto                            alpha = options.initialStep;
  unsigned int                    iter = 0u;
  apsc::LineSearch_traits::Vector nextPoint;
  nextPoint = currentPoint + alpha * searchDirection;
  bool bumped = false;
  if(this->optimizationData.bounded)
    std::tie(nextPoint, bumped) = project(nextPoint);
  Scalar nextValue = f(nextPoint);
  // iterate until sufficient decrease condition is met.
  // Some code repetition to avoid an if into a tight loop
  if(this->optimizationData.bounded)
    {
      apsc::LineSearch_traits::Vector tryPoint;
      bool                            bumped = false;
      while((nextValue >=
             currentValues.currentCostValue +
               options.sufficientDecreaseCoefficient * alpha * gradstep) and
            (iter < maxIter))
        {
          ++iter;
          alpha *= options.stepSizeDecrementFactor;
          nextPoint = currentPoint + alpha * searchDirection;
          std::tie(nextPoint, bumped) = project(nextPoint);
          nextValue = f(nextPoint);
        }
    }
  else
    {
      while((nextValue >
             currentValues.currentCostValue +
               options.sufficientDecreaseCoefficient * alpha * gradstep) and
            (iter < maxIter))
        {
          ++iter;
          alpha *= options.stepSizeDecrementFactor;
          nextPoint = currentPoint + alpha * searchDirection;
          nextValue = f(nextPoint);
        }
    }

  int status = iter < maxIter ? 0 : 2;
  return {nextPoint, nextValue, status};
}

std::tuple<apsc::LineSearch_traits::Vector, bool>
apsc::LinearSearchSolver::project(
  apsc::LineSearch_traits::Vector const &newPoint) const
{
  std::size_t                     i = 0u;
  apsc::LineSearch_traits::Vector res = newPoint;
  bool                            bumped = true;
  for(auto &x : res)
    {
      x = std::clamp(x, this->optimizationData.lowerBounds[i],
                     this->optimizationData.upperBounds[i]);
      bumped = bumped and (x == this->optimizationData.lowerBounds[i] or
                           x == this->optimizationData.upperBounds[i]);
      ++i;
    }
  return {res, bumped};
}

void
apsc::LinearSearchSolver::projectDirection(
  apsc::LineSearch_traits::Vector &searchDirection) const
{
  for(auto i = 0u; i < this->optimizationData.lowerBounds.size(); ++i)
    {
      if(this->currentValues.currentPoint[i] == optimizationData.lowerBounds[i])
        searchDirection[i] = std::max(searchDirection[i], 0.);
      if(this->currentValues.currentPoint[i] == optimizationData.upperBounds[i])
        searchDirection[i] = std::min(searchDirection[i], 0.);
    }
}
