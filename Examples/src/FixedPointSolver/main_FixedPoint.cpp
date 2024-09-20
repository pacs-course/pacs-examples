/*
 * test_FixedPoint.cpp
 *
 *  Created on: Feb 1, 2020
 *      Author: forma
 */
#include "Eigen/Dense"
#include "FixedPointIteration.hpp"
#include "FixedPointTraits.hpp"
#include <cmath>
#include <iostream>
#include <set>
#include <string>

template <class T>
void
print_result(T solution)
{
  auto [value, iter, dist, converged] = solution;
  std::cout.precision(10);
  std::cout << "The fixed point iteration ";
  if(converged)
    {
      std::cout << " converged ";
    }
  else
    {
      std::cout << " has not converged ";
    }
  std::cout << " in " << iter
            << " Iterations. Last computed values:" << value[0] << ", "
            << value[1] << std::endl;
}
int
main()
{
  using namespace apsc;
  using FixedPointIterator =
    apsc::FixedPointIteration<FixedPointArgumentType::EIGEN>;
  using IterationFunction = FixedPointIterator::IterationFunction;

  IterationFunction phi;
  // A simple iterator function that we know converges to (y(lambda), 0.739085)
  // where y(lambda)=0 if  |lambda|<1
  // if |lambda|<1 convergence to 0 , slow if |lambda| near to 1
  // at least for the non accelerated version
  // if |lambda| =1 the fixed point (0,0.739085) is unstable. In general, we do
  // not converge. if |lambda| >1 we have two fixed points, one of which is with
  // y=0 and is unstable, the other is stable
  //                and we converge to the second one
  // Try to change lambda and see what happens
  double lambda = 1;
  std::cout << "Give a lambda>=0; <1 for convergence to (0,0.7390085)\n";
  std::cin >> lambda;
  std::cout << "You have chosen lambda=" << lambda << "\n";
  std::pair<double, double> exact;
  using ArgumentType = FixedPointIterator::ArgumentType;
  phi = [lambda](ArgumentType const &x) -> ArgumentType {
    return FixedPointIterator::ArgumentType{{lambda * std::sin(x[0])},
                                            {std::cos(x[1])}};
  };

  std::set<std::string> accelerators{"NoAcceleration", "ASecant", "Anderson"};
  std::string           accelerator;
  bool                  ok = false;
  unsigned int          m{0};
  while(not ok)
    {
      std::cout << "Which acceleration do you want? (NoAcceleration, ASecant "
                   "or Anderson)"
                << std::endl;
      std::cin >> accelerator;
      if(accelerators.find(accelerator) != accelerators.end())
        {
          ok = true;
          if(accelerator == "Anderson")
            {
              std::cout << "Give the number of stored vectors for Anderson\n";
              std::cin >> m;
              std::cout << "You have chosen " << m << " vectors\n";
            }
        }
      else
        {
          std::cout << "Wrong accelerator name, retry\n";
        }
    }
  apsc::FixedPointOptions options;
  FixedPointIterator      iterate{phi, options, accelerator, m};

  ArgumentType startingPoint{
    {{5.0}, {7.0}}}; // strange way of construction a Eigen vector

  std::cout << "*** Result:\n";
  print_result(iterate.compute(startingPoint));
  return 0;
}
