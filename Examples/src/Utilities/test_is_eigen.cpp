/*
 * test_is_complex.cpp
 *
 *  Created on: Dec 27, 2020
 *      Author: forma
 */
#include <iostream>
#include "is_eigen.hpp"
int main()
{
  using namespace apsc::TypeTraits;
  Eigen::Matrix<double, 2,2 > A;
  A<<1,2,3,4;
  double c=9.0;
  std::cout<<c<<" is eigen="<<std::boolalpha<<is_eigen<decltype(c)>()<<std::endl;
  std::cout<<A<<" is eigen="<<std::boolalpha<<is_eigen<decltype(A)>()<<std::endl;
  // Now using the traits value directly
  std::cout<<c<<" is eigen="<<std::boolalpha<<is_eigen_v<decltype(c)><<std::endl;
  std::cout<<A<<" is eigen="<<std::boolalpha<<is_eigen_v<decltype(A)><<std::endl;

}




