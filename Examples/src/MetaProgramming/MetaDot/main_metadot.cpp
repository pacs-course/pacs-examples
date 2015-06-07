#include "metadot.hpp"
#include <iostream>
int main(){
  using std::array;
  array<double,5> a={1.,1.,1.,1.,1.};
  array<double,5> b={1.,2.,3.,4.,5.};
  std::cout<<"a*b="<<metaDot<5>::apply(a,b)<<std::endl;
  // Using overloaded operator
  std::cout<<"a*b="<<a*b<<std::endl;

}
