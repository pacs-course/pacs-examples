#include "refwrap.hpp"
#include <utility> 
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
int main()
{
  // This example is pratically useless. It is here only to show ref()
  // In more practical situations, if you ave a function whose
  // argument are passed by value, you may want to use ref() of cref()
  // to save a copy. But beware you may change the value of the
  // argument (unless you use cref(), but then if the function bchanges
  // the value of the parameter you have a compiler error.
  
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
