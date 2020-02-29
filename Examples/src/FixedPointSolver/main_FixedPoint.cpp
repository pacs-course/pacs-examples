/*
 * test_FixedPoint.cpp
 *
 *  Created on: Feb 1, 2020
 *      Author: forma
 */
#include <cmath>
#include <iostream>
#include "FixedPointIteration.hpp"
#include "Eigen/Dense"
#include "FixedPointTraits.hpp"
template<class T>
void print_result(T solution)
{
  auto [value,iter,dist,converged] = solution;
  std::cout.precision(10);
  std::cout<<"The fixed point iteration ";
  if (converged)
    {
      std::cout<<" converged ";
    }
  else
    {
      std::cout<<" has not converged ";
      
    }
  std::cout<<" in "<< iter<<" Iterations. Last computed values:"<<value[0]<<", "<<value[1]<<std::endl;
}
int main()
  {
    using namespace apsc;
    using FixedPointIterator=apsc::FixedPointIteration<FixedPointArgumentType::VECTOR>;
    using IterationFunction=FixedPointIterator::IterationFunction;

    IterationFunction phi;
    // A simple iterator function that we know converges to (y(lambda), 0.739085)
    // where y(lambda)=0 if  |lambda|<1
    // if |lambda|<1 convergence to 0 , slow if |lambda| near to 1
    // at least for the non accelerated version
    // if |lambda| =1 the fixed point (0,0.739085) is unstable. In general, we do not converge.
    // if |lambda| >1 we have two fixed points, one of which is with  y=0 and is unstable, the other is stable
    //                and we converge to the second one
    // Try to chenge lambda and see what happens
    double lambda=0.9;
    phi=[lambda](FixedPointIterator::ArgumentType const & x){return std::vector<double>{lambda*std::sin(x[0]),std::cos(x[1])}; };
    
    FixedPointIterator iterate{phi};
    FixedPointIterator::ArgumentType startingPoint{5.0,7.0};
    std::cout<<"*** WITH BASIC METHOD:\n";
    print_result(iterate.compute(startingPoint));
    // Now with acceleration and Eigen
    using FixedPointIterator2=apsc::FixedPointIteration<FixedPointArgumentType::EIGEN,ASecantAccelerator>;
    using IterationFunction2=FixedPointIterator2::IterationFunction;
    IterationFunction2 phi2;
    phi2 = [lambda](FixedPointIterator2::ArgumentType const & x)
                     {
                       FixedPointIterator2::ArgumentType res(2);
                       res[0]=lambda*std::sin(x[0]);
                       res[1]=std::cos(x[1]);
                       return res;
                     };
    FixedPointIterator2 iterate2{phi2};
    FixedPointIterator2::ArgumentType startingPoint2(2);
    startingPoint2[0]=5.0;
    startingPoint2[1]=7.0;
    std::cout<<"*** WITH SECANT ACCELERATION:\n";
    print_result(iterate2.compute(startingPoint2));
  }



