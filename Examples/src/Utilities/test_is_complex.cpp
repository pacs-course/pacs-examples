/*
 * test_is_complex.cpp
 *
 *  Created on: Dec 27, 2020
 *      Author: forma
 */
#include "is_complex.hpp"
#include <iostream>
// Test for complex algument using constrained oute
void
fun(apsc::TypeTraits::Complex auto const &x)
{
  std::cout << "x=" << x << std::endl;
}
// With classic template
template <apsc::TypeTraits::Complex T>
auto
fun2(T const &x)
{
  return x * x;
}

int
main()
{
  using namespace apsc::TypeTraits;
  std::complex<double> a{1., 3.};
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
  fun(a); //
  std::cout << fun2(a) << std::endl;
  // fun2("hello"); fails since char* is not a complex
 // fun2(8.); fails even if double->complex<double> is implicit
}
