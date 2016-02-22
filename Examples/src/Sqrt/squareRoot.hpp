#ifndef HH_SQUAREROOTHPP_HH
#define HH_SQUAREROOTHPP_HH
#include <limits> // for numeric_limits
#include <type_traits> // for is_floating_point
#include <algorithm> // for max and abs
#include <complex>
//! Calculation of square root by Newton method
/*!
  It calculates the square root of a number by Newton method
  until the residual is less than a tolerance.
  The tolerance is set to 2*x*machine_epsilon
  
  A max iteration counter has been implemented to avoid possible
  stagnation of the algorithm. 

  @param T floating point type
  @param x value to compute the square roor
  @return the square root of x
*/
template <typename T>
T squareRoot(const T& x)
{
  static_assert(std::is_floating_point<T>::value,"You must pass a floating point");
  constexpr unsigned int maxIter=10000;
  unsigned int counter{0};
  constexpr auto machine_eps=std::numeric_limits<T>::epsilon();
  constexpr T    two{2};
  // max to avoid having zero as tolerance
  auto my_eps=two*std::max(machine_eps,std::abs(x)*machine_eps);
  T r{x};
  while(std::abs((r * r)-x) > my_eps && counter++ < maxIter)
    {
      r = (r + x/r) / two;
    }
  return r;
}

// Version that computes the square root also of negative numbers
template <typename T>
std::complex<T> squareRootC(const T& x)
{
  static_assert(std::is_floating_point<T>::value,"You must pass a floating point");
  if (x < static_cast<T>(0.0))
    return {0,squareRoot(-x)};
  else
    return {squareRoot(x),0};
}


#endif
