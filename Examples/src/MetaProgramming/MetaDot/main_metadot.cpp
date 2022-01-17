#include "metadot.hpp"
#include <complex>
#include <iostream>
int
main()
{
  using std::array;
  using namespace apsc::Operators;
  array<double, 5> a = {1., 1., 1., 1., 1.};
  array<double, 5> b = {1.1, 2.1, 3.1, 4.1, 5.1};
  // std::cout<<"a*b="<<metaDot<5>::apply(a,b)<<std::endl;
  std::cout << "a*b=" << metadot(a, b) << std::endl;
  // with arrays of different types
  array  c{1, 2, 3, 4, 5};
  std::cout << "c*b=" << metadot(c, b) << std::endl;
  std::cout << "Now with complex numbers" << std::endl;
  array<std::complex<double>, 3> ac = {{{1., 2.3}, {-1., 7.}, {4.5, -9.0}}};
  array<std::complex<double>, 3> bc = {{{-1., 3.0}, {-2., 7.3}, {4., 10.0}}};
  std::cout << "ac*bc=" << metadot(ac, bc) << std::endl;
}
