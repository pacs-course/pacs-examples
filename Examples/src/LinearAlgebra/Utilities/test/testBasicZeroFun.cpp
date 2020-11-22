/*
 * testBasicZeroFun.cpp
 *
 *  Created on: Nov 21, 2020
 *      Author: forma
 */
#include "../basicZeroFun.hpp"
#include <functional>
#include <iostream>
int
main()
{
  // A function whose zero is known
  std::function<double(double)> fun;
  fun = [](double x) { return x * x * x - 1; };
  double a = 0;
  double b = 3;
  {
    auto res=apsc::bisection(fun,a,b,1.e-4);
    std::cout<<"Bisection result "<<res<<std::endl;
  }
  {
    auto res=apsc::chord(fun,a,b,1.e-4);
    std::cout<<"Chord result "<<res<<std::endl;
  }
  {
    b=-1;
    auto [res,status] = apsc::secant(fun,a,b);
    std::cout<<"Secant result "<<res<<" Status"<<status<<std::endl;
    }


}
