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
  /*
   * auto nonLinFun= [](const ArgumentType & x) -> ArgumentType
    {
    ArgumentType y(2);
    y(0)=(1.-x(0))*(1-x(0))*(5.-x(1))+0.1*(5.-x(1))*(5.-x(1));
    y(1)= 1.5- x(0)-0.1*x(1);
    return y;
    };

  auto jacobianFun = [](const ArgumentType & x)->JacobianType
      {
    JacobianType J(2,2);
    J(0,0)=-2*(1-x(0))*(5-x(1));
    J(0,1)=-0.2*(5-x(1))-(1.-x(0))*(1-x(0));
    J(1,0)= -1;
    J(1,1)= -0.1;
    return J;
  };
  */

     auto nonLinFun= [](const ArgumentType & x) -> ArgumentType
    {
    constexpr double pi=3.141592653589793238;
    ArgumentType y(3);
    y(0)=3*x[0]-std::cos(x[1]*x[2])-0.5;
    y(1)=x[0]*x[0]-81*(x[1]+0.1)*(x[1]+0.1)+std::sin(x[2])+1.06;
    y(2)=std::exp(-x[0]*x[1])+20*x[2]+(10.*pi-3.)/3;
    return y;
    };

  auto jacobianFun = [](const ArgumentType & x)->JacobianType
      {
    JacobianType J(3,3);
    J(0,0)=3.;
    J(0,1)=x[2]*std::sin(x[1]*x[2]);
    J(0,2)=x[1]*sin(x[1]*x[2]);
    J(1,0)=2.*x[0];
    J(1,1)=-162.*(x[1]+0.1);
    J(0,2)=std::cos(x[2]);
    J(2,0)=-x[1]*exp(-x[0]*x[1]);
    J(2,1)=-x[0]*std::exp(x[0]*x[1]);
    J(2,2)=20.;
   return J;
 };

  /* auto nonLinFun= [](const ArgumentType & x) -> ArgumentType
  {
  constexpr double pi=3.141592653589793238;
  ArgumentType y(1);
  y(0)=3*x[0]-std::cos(x[0])-0.5;
  return y;
  };

auto jacobianFun = [](const ArgumentType & x)->JacobianType
    {
  JacobianType J(1,1);
  J(0,0)=3+std::sin(x[0]);
 return J;
};
*/
  apsc::FullJacobian J(jacobianFun);

  apsc::NewtonOptions newtonOptions;
  // Change something
  newtonOptions.backtrackOn=true;
  newtonOptions.stopOnStagnation=false;
  apsc::NewtonVerbose newtonSolver(nonLinFun,J,newtonOptions);

  //ArgumentType x0(2);
  //x0<< 0., 0.; // Eigen Syntax
  ArgumentType x0(3);
  x0<< 0., 0., 0.; // Eigen Syntax
  //ArgumentType x0(1);
  //x0<< 0.; // Eigen Syntax

  // Open a scope so that the structured bindings object is eventually deleted
  {
  auto [solution,resNorm, stepLen, iter, converged, stagnated] = newtonSolver.solve(x0);

  std::cout<<std::boolalpha<<"Newton Solution has converged: "<<converged<<" last iteration:"<<iter
      <<" last residual:"<<resNorm<< " has stagnated:"<<stagnated<<std::endl;
  }
// now with discrete jacobian
  newtonSolver.setJacobianPtr(apsc::make_Jacobian(apsc::DISCRETEJACOBIAN));
  {
   auto [solution,resNorm, stepLen, iter, converged, stagnated] = newtonSolver.solve(x0);

   std::cout<<std::boolalpha<<"Newton Solution with discrete jacobian has converged: "<<converged<<" last iteration:"<<iter
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
    //newtonOptions.maxBackSteps=8; // increase max backstep
    //newtonSolver.setOptions(newtonOptions);
    //apsc::IdentityJacobian I;
    //I.setLambda(0.01);
    newtonSolver.setJacobianPtr(apsc::make_Jacobian(apsc::EIROLANEVANLINNA));
    {
    auto [solution,resNorm, stepLen, iter, converged, stagnated] =newtonSolver.solve(x0);

    std::cout<<std::boolalpha<<"E-N Solution has converged: "<<converged<<" last iteration:"<<iter
          <<" last residual:"<<resNorm<< " has stagnated:"<<stagnated<<std::endl;
    }


}



