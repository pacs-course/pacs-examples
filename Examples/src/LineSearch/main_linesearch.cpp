/*
 * main_linesearch.cpp
 *
 *  Created on: Dec 28, 2020
 *      Author: forma
 */
#include "DescentDirections.hpp"
#include "DescentDirectionFactory.hpp"
#include "GradientFiniteDifference.hpp"
#include "LineSearch.hpp"
#include "LineSearchSolver.hpp"
#include <cmath>
#include <iostream>
#include <memory>
int
main()
{
  double                    b = 100;
  apsc::LineSearchOptions   lineSearchOptions;
  apsc::OptimizationOptions optimizationOptions;
  apsc::OptimizationData    optimizationData;
  using Vector = apsc::LineSearch_traits::Vector;

  // The classic Rosembrock function
  optimizationData.NumberOfVariables = 2;
  optimizationData.costFunction = [b](const Vector &x) {
    return (1 - x[0]) * (1 - x[0]) + b * std::pow(x[1] - x[0] * x[0], 2);
  };
  // f= @(x) (1 - x(1)).^2 + b*(x(2) - x(1)^2)^2;

  /*
  //Exact Gradient
  optimizationData.gradient=[b](const Vector & x)
      {
    Vector g(x.size());
    g(0)=2*(x[0]-1)+4*b*x[0]*(x[0]*x[0]-x[1]);
    g(1)=2*b*(x[1]-x[0]*x[0]);
    return g;
      };

  //df= @(x) [2*(x(1)-1)+4*b*x(1)*(x(1)^2-x(2)); 2*b*(x(2)-x(1)^2)];
*/
  // For the lazy of you
  optimizationData.gradient =
    apsc::GradientFiniteDifference{optimizationData.costFunction};

  // Load the factory
  auto const & theFactory=apsc::loadDirections();
  // Different descent directions
  // std::unique_ptr<apsc::DescentDirectionBase> descentDirectionFunction =
  // std::make_unique<apsc::GradientDirection>();
  // std::unique_ptr<apsc::DescentDirectionBase> descentDirectionFunction =
  // std::make_unique<apsc::BFGSIDirection>();
  // std::unique_ptr<apsc::DescentDirectionBase> descentDirectionFunction =
  // std::make_unique<apsc::BFGSDirection>();
  // std::unique_ptr<apsc::DescentDirectionBase> descentDirectionFunction =
  // std::make_unique<apsc::BBDirection>();
  std::unique_ptr<apsc::DescentDirectionBase> descentDirectionFunction =
      theFactory.create("BFGSDirection");

  optimizationOptions.maxIter = 4000;
  optimizationOptions.relTol = 1.e-6;
  optimizationOptions.absTol = 1.e-8;
  lineSearchOptions.initialStep = 1.0;
  // Note the move of the unique_ptr
  apsc::LinearSearchSolver solver(optimizationData,
                                  std::move(descentDirectionFunction),
                                  optimizationOptions, lineSearchOptions);

  Vector initialPoint(2);
  initialPoint[0] = -2;
  initialPoint[1] = 3;
  // solve
  solver.setInitialPoint(initialPoint);
  auto [finalValues, numIter, status] = solver.solve();

  if(status == 0)
    std::cout << "Solver converged" << std::endl;
  else
    std::cout << "Solver DID NOT converge" << std::endl;

  std::cout << "Point found=" << finalValues.currentPoint
            << "\nCost function value=" << finalValues.currentCostValue
            << "\nGradient norm=" << finalValues.currentGradient.norm()
            << "\nNumber of iterations=" << numIter << "\nStatus=" << status
            << std::endl;
}
