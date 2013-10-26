#include "refwrap.hpp"
#include <utility> 
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

int main()
{
  double a=10;
  apply(timesThree,a);
  std::cout<<"a="<<a<<std::endl;
  apply(timesThree,std::ref(a));
  std::cout<<"a="<<a<<std::endl;
  std::list<double> l{1.,2.,3.};
  // A list of reference to l
  std::vector<std::reference_wrapper<double> >v(l.begin(),l.end());
  l.back()=150.;
  for (auto i :v) std::cout<<i<<" ";
  std::cout<<std::endl;		      

}
