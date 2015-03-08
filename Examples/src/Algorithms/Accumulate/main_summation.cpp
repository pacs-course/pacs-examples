#include "KahanSummation.hpp"
#include <iostream>
#include <vector>
#include <set>
#include <cmath>
int main()
{
  using namespace Utility;
  std::cout.precision(16);
  std::vector<double> a;
  for (int i=1; i<100000;++i)a.emplace_back(std::sqrt(static_cast<double>(5*i)));
  for (int i=1; i<100000;++i)a.emplace_back(-std::sqrt(static_cast<double>(5*i)));
  
  double sum=0;
  for (auto s : a) sum+=s;
  std::cout<<"Normal sum     :"<<sum<<std::endl;

  std::cout<<"Kahan summation:"<<kahanSummation(a)<<std::endl;



  auto f=[](double const&a,double const &b)
		    {
		      return std::abs(a)<std::abs(b);
		    };
  std::multiset<double,decltype(f)> p(f);
  std::multiset<double,decltype(f)> n(f);
  for (auto s : a){if (s>0)p.insert(s);};
  for (auto s : a){if (s<0)n.insert(s);};
  auto res=kahanSummation(p)+kahanSummation(n);
  std::cout<<"Ordered summation:"<<res<<std::endl;
  
}
