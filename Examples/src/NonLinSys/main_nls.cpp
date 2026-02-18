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
#include <numbers>

int
main()
{
  using namespace apsc::NonLinSysTraits;
  constexpr double pi = std::numbers::pi_v<double>;

  // Create a system with scalar argument and vector result.
  apsc::NonLinSys<double, ScalarTraits> system;
  // Add functions to the system.
  system.addToSystem([](double const &x) { return x * x; });
  system.addToSystem([](double const &x) { return std::sin(x); });
  system.addToSystem([](double const &x) { return std::cos(x); });
  system.addToSystem([](double const &x) { return std::tan(x); });

  // Retrieve, evaluate, and replace one equation.
  auto fun = system.getFunction(2);
  std::cout << "Function 2(pi)=" << fun(pi) << std::endl;
  system.updateFunction([](double const &x) { return x * x * x; }, 2);
  fun = system.getFunction(2);
  std::cout << "Function 2(pi) after update=" << fun(pi) << std::endl;

  // Call the whole system.
  auto res = system(3.0);
  std::cout << "system(3.0)= ";
  for(auto y : res)
    std::cout << y << ",";
  std::cout << std::endl;
  // Iterate through the system.
  std::cout << "iterating on the system :\n ";
  for(auto const &f : system)
    {
      std::cout << f(3.0) << ",";
    }

  std::cout << std::endl;

  // Now use Eigen vectors as function argument type.
  apsc::NonLinSys<double, apsc::NonLinSysTraits::EigenVectorTraits> system2;
  system2.addToSystem(
    [](Eigen::VectorXd const &x) { return x.array().square().sum(); });
  system2.addToSystem(
    [](Eigen::VectorXd const &x) { return x.array().sin().sum(); });
  system2.addToSystem([](Eigen::VectorXd const &x) { return x.norm(); });
  system2.addToSystem([](Eigen::VectorXd const &x) { return x.lpNorm<1>(); });

  // Retrieve and evaluate one function.
  auto fun2 = system2.getFunction(2);
  std::cout << "Function 2(pi)=" << fun2(Eigen::VectorXd::Constant(3, pi))
            << std::endl;

  // Call the whole system.
  apsc::NonLinSys<
    double, apsc::NonLinSysTraits::EigenVectorTraits>::ArgumentType arg(3);
  arg << 1., 2., -9;
  auto res3 = system2(arg);
  std::cout << "system2(1,2,-9)= ";
  for(auto const value : res3)
    {
      std::cout << value << ",";
    }
  std::cout << std::endl;

  // Function factory example.
  apsc::FunctionFactory<double, ScalarTraits> factory;

  // Add three functions.
  factory.addToFactory("Function 1",
                       [](double const &x) { return std::sin(x); });
  factory.addToFactory("Function 2",
                       [](double const &x) { return std::cos(x); });
  factory.addToFactory("Function 3",
                       [](double const &x) { return std::tan(x); });
  // Uncommenting this would throw: duplicate identifier.
  // factory.addToFactory("Function 2", [](double const &x) { return
  // std::tan(x); });

  // Get one function by identifier.
  auto funct = factory.getFunction("Function 2");
  std::cout << "Function 2(pi)=" << funct(pi) << std::endl;

  // Also, you cannot add functions without a unique identifier:
  // factory.addToSystem([](double const &x){return std::cos(x);});
}
