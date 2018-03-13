#include "KahanSummation.hpp"
#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <numeric> // for accumulate

#ifndef REAL
#define REAL  double
#endif
// Compile with -DREAL=float if you want floats
int main()
{
  using namespace Utility;
  std::cout.precision(16);
  std::vector<REAL> a;
  // create a vector that sums to zero, startin with biggest values
  for (int i=100000; i>0;--i)a.emplace_back(std::sqrt(static_cast<REAL>(5*i)));
  for (int i=100000; i>0;--i)a.emplace_back(-std::sqrt(static_cast<REAL>(5*i)));
  
  // an example of accumulate
  auto sum = std::accumulate(a.begin(),a.end(),static_cast<REAL>(0.0));
  // but you can do also simply
  // REAL sum=0;
  // for (auto s : a) sum+=s;
  std::cout<<"Normal sum     :"<<sum<<std::endl;

  std::cout<<"Kahan summation:"<<kahanSummation(a)<<std::endl;

  // Another trick. Order the data so that smallest (in absolute value)
  // are added first

  auto f=[](REAL const&a,REAL const &b)
		    {
		      return std::abs(a)<std::abs(b);
		    };
  std::multiset<REAL,decltype(f)> p(f);
  std::multiset<REAL,decltype(f)> n(f);
  for (auto s : a){if (s>0)p.insert(s);};
  for (auto s : a){if (s<0)n.insert(s);};
  auto res=std::accumulate(p.begin(),p.end(),static_cast<REAL>(0.0))+
    std::accumulate(n.begin(),n.end(),static_cast<REAL>(0.0));
  std::cout<<"Ordered summation:"<<res<<std::endl;
  
}
