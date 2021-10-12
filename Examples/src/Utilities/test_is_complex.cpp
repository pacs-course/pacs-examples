/*
 * test_is_complex.cpp
 *
 *  Created on: Dec 27, 2020
 *      Author: forma
 */
#include "is_complex.hpp"
#include <iostream>
int
main()
{
  using namespace apsc::TypeTraits;
  std::complex<double> a{1., 1.};
  double               c = 9.0;
  std::cout << c << " is complex=" << std::boolalpha
            << is_complex<decltype(c)>() << std::endl;
  std::cout << a << " is complex=" << std::boolalpha
            << is_complex<decltype(a)>() << std::endl;
  // Now using the traits value directly
  std::cout << c << " is complex=" << std::boolalpha
            << is_complex_v<decltype(c)> << std::endl;
  std::cout << a << " is complex=" << std::boolalpha
            << is_complex_v<decltype(a)> << std::endl;
}
