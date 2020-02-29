/*
 * test_Newton.cpp
 *
 *  Created on: Feb 23, 2020
 *      Author: forma
 */
#include "Newton.hpp"
#include "JacobianFactory.hpp"
#include <iostream>
#include <iomanip>
int main()
{
  using ArgumentType = apsc::NewtonTraits::ArgumentType;
  using NonLinSysType = apsc::NewtonTraits::NonLinearSystemType;
  using JacobianType =apsc::FullJacobian::JacobianMatrixType;

  NonLinSysType nonLinSys;
  auto nonLinFun= [](const ArgumentType & x) -> ArgumentType
    {
    ArgumentType y(2);
    y(0)=(1.-x(0))*(1-x(0))+0.1*(5.-x(1))*(5.-x(1));
    y(1)= 1.5- x(0)-0.1*x(1);
    return y;
    };

  auto jacobianFun = [](const ArgumentType & x)->JacobianType
      {
    JacobianType J(2,2);
    J(0,0)=-2*(1-x(0));
    J(0,1)=-0.2*(5-x(1));
    J(1,0)= -1;
    J(1,1)= -0.1;
    return J;
  };

  apsc::FullJacobian J(jacobianFun);

  apsc::NewtonOptions newtonOptions;
  // Change something
  newtonOptions.backtrackOn=true;
  apsc::NewtonVerbose newtonSolver(nonLinFun,J,newtonOptions);

  ArgumentType x0(2);
  x0<< 0., 0.; // Eigen Syntax

  // Open a scope so that the structured bindings object is eventually deleted
  {
  auto [solution,resNorm, stepLen, iter, converged, stagnated] = newtonSolver.solve(x0);

  std::cout<<std::boolalpha<<"Newton Solution has converged: "<<converged<<" last iteration:"<<iter
      <<" last residual:"<<resNorm<< " has stagnated:"<<stagnated<<std::endl;
  }
  // Now with Broyden
  newtonOptions.maxIter=100; // increase max iterations
  newtonSolver.setOptions(newtonOptions);
  newtonSolver.setJacobianPtr(apsc::make_Jacobian(apsc::BROYDENB));
  {
  auto [solution,resNorm, stepLen, iter, converged, stagnated] =newtonSolver.solve(x0);

  std::cout<<std::boolalpha<<"Broyden (bad) Solution has converged: "<<converged<<" last iteration:"<<iter
        <<" last residual:"<<resNorm<< " has stagnated:"<<stagnated<<std::endl;
  }
  // Now with Broyden
   newtonSolver.setJacobianPtr(apsc::make_Jacobian(apsc::BROYDENG));
   {
   auto [solution,resNorm, stepLen, iter, converged, stagnated] =newtonSolver.solve(x0);

   std::cout<<std::boolalpha<<"Broyden (good) Solution has converged: "<<converged<<" last iteration:"<<iter
         <<" last residual:"<<resNorm<< " has stagnated:"<<stagnated<<std::endl;
   }
   // Now with just Richardson (Picard iterations)
    newtonOptions.maxBackSteps=8; // increase max backstep
    newtonSolver.setOptions(newtonOptions);
    newtonSolver.setJacobianPtr(apsc::make_Jacobian(apsc::IDENTITYJACOBIAN));
    {
    auto [solution,resNorm, stepLen, iter, converged, stagnated] =newtonSolver.solve(x0);

    std::cout<<std::boolalpha<<"Richardon Solution has converged: "<<converged<<" last iteration:"<<iter
          <<" last residual:"<<resNorm<< " has stagnated:"<<stagnated<<std::endl;
    }


}



