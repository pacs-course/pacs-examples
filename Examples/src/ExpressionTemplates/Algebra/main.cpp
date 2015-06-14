#include "vectorExpr.hpp"
#include "operators.hpp"
#include <iostream>
int main()
{
  using namespace ET;
  Vector a(10);
  Vector b(a);
  std::vector<double> & bb=b;
  for (auto & i : bb) i=1;
  std::cout<<bb.size()<<bb[3]<<std::endl;
  Vector c(a);
  c=a+b;
  Vector d(a+b+c);
  for (auto i : d) std::cout<<i<<" ";
  std::cout<<std::endl;
  
}
