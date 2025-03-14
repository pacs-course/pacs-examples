/*
 * main_nls.cpp
 *
 *  Created on: Feb 12, 2021
 *      Author: forma
 */
#include "FunctionFactory.hpp"
#include "NonLinSys.hpp"
#include <cmath>
#include <iostream>
#include <numeric>

int
main()
{
  using namespace apsc::NonLinSysTraits;

  // I create a system of scalar functions
  apsc::NonLinSys<double, ScalarTraits> system;
  // I add a function to the system
  system.addToSystem([](double const &x) { return x * x; });
  // I add another function
  system.addToSystem([](double const &x) { return std::sin(x); });
  // I add another function
  system.addToSystem([](double const &x) { return std::cos(x); });
  // I add another function
  system.addToSystem([](double const &x) { return std::tan(x); });

  // I can get the function with the index
  auto fun = system.getFunction(2);
  std::cout << "Function 2(3.1415)=" << fun(3.1415) << std::endl;
  // I can change the function
  system.updateFunction([](double const &x) { return x * x * x; }, 2);
  fun = system.getFunction(2);
  std::cout << "Function 2(3.1415)=" << fun(3.1415) << std::endl;
  // I can call the system
  auto res = system(3.0);
  std::cout << "system(3.0)= ";
  for(auto y : res)
    std::cout << y << ",";
  std::cout << std::endl;
  // I can iterate on a system
  std::cout << "iterating on the system :\n ";
  for(auto const &f : system)
    {
      std::cout << f(3.0) << ",";
    }

  std::cout << std::endl;

  // Now I try the case with Eigen Vector as arguments
  apsc::NonLinSys<double, apsc::NonLinSysTraits::EigenVectorTraits> system2;
  // I add a function to the system
  system2.addToSystem(
    [](Eigen::VectorXd const &x) { return x.array().square().sum(); });
  // I add another function
  system2.addToSystem(
    [](Eigen::VectorXd const &x) { return x.array().sin().sum(); });
  // I add another function
  system2.addToSystem([](Eigen::VectorXd const &x) { return x.norm(); });
  // I add another function
  system2.addToSystem([](Eigen::VectorXd const &x) { return x.lpNorm<1>(); });

  // I can get the function with the index
  auto fun2 = system2.getFunction(2);
  std::cout << "Function 2(3.1415)="
            << fun2(Eigen::VectorXd::Constant(3, 3.1415)) << std::endl;
  // I can call the system
  apsc::NonLinSys<
    double, apsc::NonLinSysTraits::EigenVectorTraits>::ArgumentType arg(3);
  arg << 1., 2., -9;
  auto res3 = system2(arg);
  std::cout << "system2(1,2,-9)=" << res3[0] << "," << res3[1] << "," << res3[2]
            << std::endl;
  // Now I try the function factory
  apsc::FunctionFactory<double, ScalarTraits> factory;

  // Add three functions
  factory.addToFactory("Function 1",
                       [](double const &x) { return std::sin(x); });
  factory.addToFactory("Function 2",
                       [](double const &x) { return std::cos(x); });
  factory.addToFactory("Function 3",
                       [](double const &x) { return std::tan(x); });
  // if uncommented I have an error you cannot have t2 functions with the same
  // id factory.addToFactory("Function 2",[](double const &x){return
  // std::tan(x);}); Get one function with the identifier
  auto funct = factory.getFunction("Function 2");
  std::cout << "Function 2(3.1415)=" << funct(3.1415) << std::endl;
  // If uncommented I have an error
  // auto fun2 = factory.getFunction("Function 3");
  // Also if I uncomment th enext line I get an error. I cannot just add a
  // function without a unique identifier
  // factory.addToSystem([](double const &x){return std::cos(x);});
}
