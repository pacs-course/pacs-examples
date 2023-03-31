/*
 * main_linesearch.cpp
 *
 *  Created on: Dec 28, 2020
 *      Author: forma
 */
#include "DescentDirectionFactory.hpp"
#include "DescentDirections.hpp"
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
  using Matrix = apsc::LineSearch_traits::Matrix;

  // The classic Rosembrock function
  optimizationData.NumberOfVariables = 2;
  /*
   *
   //Rosembrock
   optimizationData.costFunction = [b](const Vector &x) {
     return (1 - x[0]) * (1 - x[0]) + b * std::pow(x[1] - x[0] * x[0], 2);
   };
   // f= @(x) (1 - x(1)).^2 + b*(x(2) - x(1)^2)^2;


   //Exact Gradient
   optimizationData.gradient=[b](const Vector & x)
       {
     Vector g(x.size());
     g(0)=2*(x[0]-1)+4*b*x[0]*(x[0]*x[0]-x[1]);
     g(1)=2*b*(x[1]-x[0]*x[0]);
     return g;
       };
   // Exact Hessian

   optimizationData.hessian = [b](const Vector &x) {
     Matrix m(x.size(), x.size());
     m(0, 0) = 2 + 4 * b*((x[0]*x[0]-x[1])+2*x[0]*x[0]);
     m(0, 1) = -4*b*x[0];
     m(1, 0) = -4*b*x[0];
     m(1, 1) = 2 * b;
     return m;
   };
 */

  /*
   *
    // Quadratic
    optimizationData.costFunction = [b](const Vector &x) {
      return (1 - x[0]) * (1 - x[0]) + b * std::pow(x[1] - x[0], 2);
    };

    optimizationData.gradient = [b](const Vector &x) {
      Vector g(x.size());
      g(0) = 2 * (x[0] - 1) - 2 * b * (x[1] - x[0]);
      g(1) = 2 * b * (x[1] - x[0]);
      return g;
    };

    optimizationData.hessian = [b](const Vector &x) {
      Matrix m(x.size(), x.size());
      m(0, 0) = 2 + 2 * b;
      m(0, 1) = -2 * b;
      m(1, 0) = -2 * b;
      m(1, 1) = 2 * b;
      return m;
    };
   */
  // Quadratic
  optimizationData.costFunction = [b](const Vector &x) {
    return 2 - 0.5 * x[0] + std::sqrt(1. + b * x[1] * x[1] + x[0] * x[0]);
  };
  /*
   * ans(x, y) =

  x/(x^2 + b*y^2 + 1)^(1/2) - 1/2
    (b*y)/(x^2 + b*y^2 + 1)^(1/2)

  >> simplify(hessian(f, [x,y]))

  ans(x, y) =

  [(b*y^2 + 1)/(x^2 + b*y^2 + 1)^(3/2),      -(b*x*y)/(x^2 + b*y^2 + 1)^(3/2)]
  [   -(b*x*y)/(x^2 + b*y^2 + 1)^(3/2), (b*(x^2 + 1))/(x^2 + b*y^2 + 1)^(3/2)]

   */
  optimizationData.gradient = [b](const Vector &x) {
    Vector g(x.size());
    g(0) = x[0] / std::sqrt(x[0] * x[0] + b * x[1] * x[1] + 1) - 1 / 2.;
    g(1) = (b * x[1]) / std::sqrt(x[0] * x[0] + b * x[1] * x[1] + 1);
    return g;
  };

  optimizationData.hessian = [b](const Vector &x) {
    Matrix m(x.size(), x.size());
    auto   v = 1. / std::pow(x[0] * x[0] + b * x[1] * x[1] + 1, 3. / 2);
    m(0, 0) = (b * x[1] * x[1] + 1) * v;
    m(0, 1) = -(b * x[0] * x[1] + 1) * v;
    m(1, 0) = m(0, 1);
    m(1, 1) = (b * (x[0] * x[0] + 1)) * v;
    return m;
  };

  // df= @(x) [2*(x(1)-1)+4*b*x(1)*(x(1)^2-x(2)); 2*b*(x(2)-x(1)^2)];

  // For the lazy of you
  //  optimizationData.gradient =
  //    apsc::GradientFiniteDifference{optimizationData.costFunction};

  // Load the factory
  auto const &theFactory = apsc::loadDirections();
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
    theFactory.create("NewtonDirection");

  optimizationOptions.maxIter = 4000;
  optimizationOptions.relTol = 1.e-8;
  optimizationOptions.absTol = 1.e-8;
  lineSearchOptions.initialStep = 1.0;
  setBounds(optimizationData, {0., 0.}, {1.0, 1.0});
  if(optimizationData.bounded)
    {
      std::cout << "Bounds:\n";
      std::cout << "Lower: " << optimizationData.lowerBounds[0] << " "
                << optimizationData.lowerBounds[1] << "\n";
      std::cout << "Upper: " << optimizationData.upperBounds[0] << " "
                << optimizationData.upperBounds[1] << "\n";
    }
  // Note the move of the unique_ptr
  apsc::LinearSearchSolver solver(optimizationData,
                                  std::move(descentDirectionFunction),
                                  optimizationOptions, lineSearchOptions);

  Vector initialPoint(2);
  if(optimizationData.bounded)
    {
      initialPoint[0] =
        (optimizationData.lowerBounds[0] + optimizationData.upperBounds[0]) /
        2.;
      initialPoint[1] =
        (optimizationData.lowerBounds[1] + optimizationData.upperBounds[1]) /
        2.;
    }
  else
    {
      initialPoint[0] = 0.5;
      initialPoint[1] = 0.5;
    }
  // solve
  solver.setInitialPoint(initialPoint);
  auto [finalValues, numIter, status] = solver.solve();

  if(status == 0)
    std::cout << "Solver converged" << std::endl;
  else
    std::cout << "Solver DID NOT converge" << std::endl;

  std::cout << "Point found=" << finalValues.currentPoint
            << "\nCost function value=" << finalValues.currentCostValue
            << "\nGradient   =" << finalValues.currentGradient
            << "\nGradient norm=" << finalValues.currentGradient.norm()
            << "\nNumber of iterations=" << numIter << "\nStatus=" << status
            << std::endl;
}
