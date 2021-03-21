/*
 * main_nls.cpp
 *
 *  Created on: Feb 12, 2021
 *      Author: forma
 */
#include "NonLinSys.hpp"
#include "FunctionFactory.hpp"
#include<iostream>
#include<numeric>
#include <cmath>

int main()
{
  using namespace apsc::NonLinSysTraits;
  // The greater common division and largest common multiplies with 2 to 100
  apsc::NonLinSys<unsigned int,ScalarTraits> nls;
  // I extract the type from the non linear system (not really necessary, I know it is an unsigned int)
  using ArgumentType=apsc::NonLinSys<unsigned int,ScalarTraits>::ArgumentType;
  for (unsigned int i=2; i<=100u;++i)
    {
      nls.addToSystem([i](ArgumentType const & x){return std::gcd(x,i);} );
    }
  // gcd/lcm of 333 with 11
  unsigned int res1 = nls(10,333);
  auto res2 = nls(333);
  std::cout<<"gcd 333  and 11 "<<res1<<std::endl;

  std::size_t k=0;
  for (std::size_t i=2;i<=100u;++i)
    {
      auto z=res2[k++];
      if (z==1u)
        std::cout<<"333 and "<<i<<" are co-prime\n";
    }
  std::cout<<std::endl;

  // The case of a system of two equation of two variable. I use the default trait
  apsc::NonLinSys<double> nlsd;
  using ArgumentTyped=apsc::NonLinSys<double>::ArgumentType;
  nlsd.addToSystem([] (ArgumentTyped const & x){return std::sin(x[0])+std::cos(x[1]);});
  nlsd.addToSystem([] (ArgumentTyped const & x){return std::sinh(x[0])+std::cosh(x[1]);});
  auto res=nlsd({3.4, 5.7}); // pass the vector 3.4, 5.7
  std::cout<<"Result of the call to the system of functions:\n";
  for (auto y:res)
    {
      std::cout<<y<<" ";
    }
  std::cout<<std::endl;

  // Now I try the function factory
  apsc::FunctionFactory<double,ScalarTraits> factory;

  // Add two functions
  factory.addToFactory("Function 1",[](double const &x){return std::sin(x);});
  factory.addToFactory("Function 2",[](double const &x){return std::cos(x);});
  // if uncomemnted I have an error
  // factory.addToFactory("Function 2",[](double const &x){return std::tan(x);});
  // Get one function with the identifier
  auto fun = factory.getFunction("Function 2");
  std::cout<<"Function 2(3.1415)="<<fun(3.1415)<<std::endl;
  // If uncommented I have an error
  //auto fun2 = factory.getFunction("Function 3");
  // Also if I uncomment th enext line I get an error. I cannot just add a function without a unique identifier
  //factory.addToSystem([](double const &x){return std::cos(x);});



}



