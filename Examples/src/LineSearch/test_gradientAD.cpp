
#include "GradientsAD.hpp"
#include <iostream>
/*
This is a simple test of the GradientsAD class, which computes the cost,
gradient and Hessian of a given cost function using automatic differentiation.
The cost function is the classic Rosembrock function, which has a minimum at
(1,1). The test evaluates the cost, gradient and Hessian at the point
(-1.2, 1.0), which is a common starting point for optimization algorithms.

I try all different possibility offered by the class, including evaluating the
cost, gradient and Hessian separately, evaluating them all at once, and building
callable functions for the cost, gradient and Hessian.

The expected output  (for b=100, x-0=(-1.2, 1.0)))   is:
Cost value: 24.2
Gradient: -215.6 088
Hessian:
  1330 480
  480 200
*/
int
main()
{
  using Vector = apsc::LineSearch_traits::Vector;
  using Matrix = apsc::LineSearch_traits::Matrix;
  using Scalar = apsc::LineSearch_traits::Scalar;
  using ArgumentType = apsc::GradientsAD::AutoDiffArgumentType;
  using CostFunction = apsc::GradientsAD::AutodiffCostFunction;

  // The classic Rosembrock function
  double       b = 100;
  CostFunction costFunction = [b](const ArgumentType &x) {
    return (1 - x[0]) * (1 - x[0]) + b * pow(x[1] - x[0] * x[0], 2);
  };

  // Compute gradients and Hessian using automatic differentiation at point
  // x=(-1.2, 1.0)

  apsc::GradientsAD gradients(costFunction);

  Vector x(2);
  x << -1.2, 1.0;

  Scalar costValue = gradients.evaluateCost(x);
  Vector gradient = gradients.evaluateGradient(x);
  Matrix hessian = gradients.evaluateHessian(x);

  std::cout << "Cost value: " << costValue << std::endl;
  std::cout << "Gradient: " << gradient.transpose() << std::endl;
  std::cout << "Hessian:\n" << hessian << std::endl;

  std::cout << "Evaluating all at once:" << std::endl;
  auto [costValue2, gradient2, hessian2] = gradients.evaluateAll(x);
  std::cout << "Cost value: " << costValue2 << std::endl;
  std::cout << "Gradient: " << gradient2.transpose() << std::endl;
  std::cout << "Hessian:\n" << hessian2 << std::endl;
  // The output should be:
  // Cost value: 24.199999999999996
  // Gradient: -215.6 88.0
  // Hessian:
  //  2158.0 -400.0
  // -400.0 200.0

  std::cout
    << "Now by building the callable functions for cost, gradient and Hessian:"
    << std::endl;
  auto costFunc = gradients.theCostFunction();
  auto gradFunc = gradients.theGradientFunction();
  auto hessFunc = gradients.theHessianFunction();
  std::cout << "Cost value: " << costFunc(x) << std::endl;
  std::cout << "Gradient: " << gradFunc(x).transpose() << std::endl;
  std::cout << "Hessian:\n" << hessFunc(x) << std::endl;
  return 0;
}
