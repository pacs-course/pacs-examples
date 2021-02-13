/*
 * main_nls.cpp
 *
 *  Created on: Feb 12, 2021
 *      Author: forma
 */
#include "NonLinSys.hpp"
#include<iostream>
#include<numeric>
#include <cmath>
// Here  we show how to change traits to have a scalar as argument and not a vector
template<class Scalar>
struct ScalarTrait
    {
      using ArgumentType=Scalar;
      using ResultType=std::vector<Scalar>;
      using ScalarType=Scalar;
      using ScalarFunctionType = std::function<ScalarType (ArgumentType const &)>;
      using VectorFunctionType = std::function<ResultType (ArgumentType const &)>;
      using SystemType = std::vector<ScalarFunctionType>;
    };

int main()
{
// The greater common division and largest common multiplies with 2 to 100
apsc::NonLynSys<unsigned int,ScalarTrait> nls;
// I extract the type from the non linear system (not really necessary, I know it is an unsigned int)
using ArgumentType=apsc::NonLynSys<unsigned int,ScalarTrait>::ArgumentType;
for (unsigned int i=2; i<=100u;++i)
  {
    nls.addToSystem([i](ArgumentType const & x){return std::gcd(x,i);} );
  }
// gcd/lcm of 333 with 11
  unsigned int res1 = nls(10u,333);
  auto res2 = nls(333);
  std::cout<<"gcd 333  and 11 "<<res1<<std::endl;

  std::size_t k=0;
  for (std::size_t i=2;i<100u;++i)
    {
      auto z=res2[k++];
      if (z==1u)
        std::cout<<"333 and "<<i<<" are co-prime\n";
    }
  std::cout<<std::endl;

// The case of a system of two equation of two variable. I use the default trait
  apsc::NonLynSys<double> nlsd;
  using ArgumentTyped=apsc::NonLynSys<double>::ArgumentType;
  nlsd.addToSystem([] (ArgumentTyped const & x){return std::sin(x[0])+std::cos(x[1]);});
  nlsd.addToSystem([] (ArgumentTyped const & x){return std::sinh(x[0])+std::cosh(x[1]);});
  auto res=nlsd({3.4, 5.7}); // pass the vector 3.4, 5.7
  for (auto y:res)
    {
      std::cout<<y<<" ";
    }
 std::cout<<std::endl;




}



