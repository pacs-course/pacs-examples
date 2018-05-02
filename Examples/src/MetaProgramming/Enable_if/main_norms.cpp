#include "Norms.hpp"
#include <iostream>
int main()
{
  std::vector<double> a{1.0,2.3,4.5};
  std::vector<std::complex<double>> c{{1.0,2.0},{6.0,4.5}};
  Norm2 norm;
  auto n1 = norm(a);
  auto n2 = norm(c);
  std::cout<<" norm of a="<<n1<<std::endl;
  std::cout<<" norm of c="<<n2<<std::endl;
  std::vector<int> i{1,2,3};
  //norm(i);/// compilation error
}
