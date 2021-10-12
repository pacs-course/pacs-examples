/*! \mainpage Expression Template example
 *
 * this example deals with Expression Templates.<br>
 *
 * they are used to perform integration of a user defined expression in a
 * "natural" way, using different integration schemes.
 *
 */

#include <cmath>
#include <iostream>

#include "expr.hpp"
#include "integrate.hpp"
#include "operation.hpp"
#include "var.hpp"

using namespace ET;

int
main()
{
  // define a variable
  Var::Identity<> x;

  // declare an expression
  auto expr = (x + 2.) * 3.; // << c++11 here!

  // evaluate the expression in a given point
  std::cout << eval(expr, 3.) << std::endl;

  // integrate e^x on [0.,1.] with the midpoint rule on 5 intervals
  std::cout << Integrate::midPoint(exp(x), 0., 1., 5) << std::endl;

  // check convergence of the midpoint rule wrt an analytical solution
  double analyticalValue = exp(1.) - exp(0.);
  std::cout << "Midpoint:" << std::endl;
  for(int n = 10; n < 100; n *= 2)
    {
      double intValue = Integrate::midPoint(exp(x), 0., 1., n);
      double error = std::fabs(intValue - analyticalValue);
      std::cout << "error with " << n << " intervals: " << error << std::endl;
    }

  std::cout << "Simpson:" << std::endl;
  for(int n = 10; n < 100; n *= 2)
    {
      double intValue = Integrate::simpson(exp(x), 0., 1., n);
      double error = std::fabs(intValue - analyticalValue);
      std::cout << "error with " << n << " intervals: " << error << std::endl;
    }

  // integrate x / 2 - 3  on [0.,1.] with 10 intervals
  std::cout << Integrate::trapez(x / 2. - 3., 0., 1., 10) << std::endl;

  return 0;
}
