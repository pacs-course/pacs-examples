/*
 * main_nls.cpp
 *
 *  Created on: Feb 12, 2021
 *      Author: forma
 */
#include "NonLinSys.hpp"
#include<iostream>
#include<numeric>
int main()
{
// The greater common division and largest common multiplies with 2 to 100
apsc::NonLynSys<unsigned int> nls;
for (unsigned int i=2; i<=100u;++i)
  {
    nls.addToSystem([i](std::vector<unsigned int> const & x){return std::gcd(x[0],i);} );
  }
// gcd/lcm of 333 with 11
  unsigned int res1 = nls(10u,{333});
  auto res2 = nls({333});
  std::cout<<"gcd 333  and 11 "<<res1<<std::endl;

  std::size_t k=0;
  for (std::size_t i=2;i<100u;++i)
    {
      auto z=res2[k++];
      if (z==1u)
        std::cout<<"gcd 333 and "<<i<<" are co-prime\n";
    }
  std::cout<<std::endl;



}



