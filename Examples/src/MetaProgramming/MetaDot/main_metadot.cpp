#include "metadot.hpp"
#include <iostream>
int main(){
  using std::array;
  array<double,5> a={1.,1.,1.,1.,1.};
  array<double,5> b={1.1,2.1,3.1,4.1,5.1};
  //std::cout<<"a*b="<<metaDot<5>::apply(a,b)<<std::endl;
  // Using overloaded operator
  std::cout<<"a*b with the first implementation"<<std::endl;
  std::cout<<"a*b="<<a*b<<std::endl;
  std::cout<<"a*b with the second implementation"<<std::endl;
  std::cout<<"a*b="<<metadot17(a,b)<<std::endl;
  // the second implementation morks with arrays of different types
  array<int,5> c={1,2,3,4,5};
  std::cout<<"b*c with the second implementation"<<std::endl;
  std::cout<<"c*b="<<metadot17(c,b)<<std::endl;
}
