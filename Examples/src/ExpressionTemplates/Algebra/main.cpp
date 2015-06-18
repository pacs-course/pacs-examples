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
  Vector c(a);
  c=a+b;
  std::cout<<a.size()<<" "<<b.size()<<" "<<c.size()<<std::endl;
  Vector d(a+b+c);
  //for (auto i : d) std::cout<<i<<" ";
  //std::cout<<std::endl;
  std::vector<double> z(11,3.0);
  Vector zv(z);
  std::vector<double> & zr(zv);
  zr.emplace_back(5.0);
  for (auto i : zv) std::cout<<i<<" ";
  std::cout<<std::endl;
  
  std::vector<double> & zrr{zv.as_vector()};
  zrr.emplace_back(6.0);

  for (auto i : zv) std::cout<<i<<" ";
  std::cout<<std::endl;
  
  a=3.0*zv; //multiplication by a scalar

  a=-a;
  for (auto i : a) std::cout<<i<<" ";
  std::cout<<std::endl;

  // Formula involvind exponent

  a=c+8.0*ET::exp(b);
  for (auto i : a) std::cout<<i<<" ";
  std::cout<<std::endl;

}
