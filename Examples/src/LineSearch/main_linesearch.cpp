/*
 * main_linesearch.cpp
 *
 *  Created on: Dec 28, 2020
 *      Author: forma
 */
#include "DescentDirectionFactory.hpp"
#include "DescentDirections.hpp"
#include "GradientFiniteDifference.hpp"
#include "GradientsAD.hpp"
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

  // Check if a file named "linesearch_options.json" exists in the current
  // directory, and if so, read the options from it
  std::ifstream file("linesearch_options.json");
  if(file.is_open())
    {
      lineSearchOptions.readFromFile("linesearch_options.json");
    }
  else
    {
      std::cout
        << "No linesearch_options.json file found, using default options."
        << std::endl;
    }
  // check if a file named "optimization_options.json" exists in the current
  // directory, and if so, read the options from it
  std::ifstream optFile("optimization_options.json");
  if(optFile.is_open())
    {
      optimizationOptions.readFromFile("optimization_options.json");
    }
  else
    {
      std::cout
        << "No optimization_options.json file found, using default options."
        << std::endl;
    }

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
   // a quadratic function with a square root
   optimizationData.costFunction = [b](const Vector &x) {
     return 2 - 0.5 * x[0] + std::sqrt(1. + b * x[1] * x[1] + x[0] * x[0]);
   };

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
   */
  // If you want tu use AD to compute the gradient and Hessian, uncomment the
  // following lines
  // note: you have to use sqrt and not std::sqrt in the cost function, because
  // autodiff::sqrt is overloaded
  apsc::GradientsAD::AutodiffCostFunction costFunctionAD =
    [b](const apsc::GradientsAD::AutoDiffArgumentType &x) {
      return 2 - 0.5 * x[0] + sqrt(1. + b * x[1] * x[1] + x[0] * x[0]);
    };
  apsc::GradientsAD gradientsAD(costFunctionAD);
  optimizationData.costFunction = gradientsAD.theCostFunction();
  optimizationData.gradient = gradientsAD.theGradientFunction();
  optimizationData.hessian = gradientsAD.theHessianFunction();
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
  std::string theDirection = lineSearchOptions.descentDirection;
  std::unique_ptr<apsc::DescentDirectionBase> descentDirectionFunction;
  try
    {
      descentDirectionFunction = theFactory.create(theDirection);
    }
  catch(const std::exception &e)
    {
      std::cerr << "Error creating descent direction: " << e.what()
                << std::endl;
      std::cout << "Available descent directions: ";
      for(auto const &name : theFactory.registered())
        {
          std::cout << name << " ";
        }
      std::cout << std::endl;
      return 1;
    }

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
  // print current parameters for cjheck
  std::cout << lineSearchOptions << std::endl;
  std::cout << optimizationOptions << std::endl;
  // solve
  solver.setInitialPoint(initialPoint);
  auto [finalValues, numIter, status] = solver.solve();

  if(status == 0)
    std::cout << "Solver converged" << std::endl;
  else
    std::cout << "Solver DID NOT converge" << std::endl;

  std::cout << "Point found=" << finalValues.currentPoint.transpose()
            << "\nCost function value=" << finalValues.currentCostValue
            << "\nGradient   =" << finalValues.currentGradient.transpose()
            << "\nGradient norm=" << finalValues.currentGradient.norm()
            << "\nNumber of iterations=" << numIter << "\nStatus=" << status
            << std::endl;
}
