#include "refwrap.hpp"
#include <utility> 
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
int main()
{
  double a=10;
  apply(timesThree,a);
  // a has not chenged since it is passed by value!
  std::cout<<"a="<<a<<std::endl;
  apply(timesThree,std::ref(a));
  // Now a has changed
  std::cout<<"a="<<a<<std::endl;
  std::list<double> l{1.,2.,3.};
  // A vector of references to elements of l
  std::vector<std::reference_wrapper<double> >v(l.begin(),l.end());
  // Change the last element of l
  l.back()=150.;
  // also the last element of v has changes, beig a reference
  // wrapper!
  for (auto i :v) std::cout<<i<<" ";
  std::cout<<std::endl;		      

}
