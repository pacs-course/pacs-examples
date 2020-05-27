#include "is_complex.hpp"
#include <type_traits>
#include <vector>
#include <complex>
#include <cmath>
#ifndef HH_NORMS_HH
#define HH_NORMS_HH
//! Computes the 2-norm of a std:;vector<double> or of a std::vector<std::complex<double>>
/*! 
  An example of the use of enable_if to select a specific function member depending on the type
  of the template argument.

  In this case the SFINAE paradigm is activated on the return type of the call operator.

  <b>Take it only as an example</b>. The function std::norm works also for floating point types, so you do not
  need to have two separate call operators, you can just use the second version (taking away the enable_if stuff of course)
  
 */
struct Norm2
{
  //! For vector<double>
  template<class C>
  std::enable_if_t<std::is_floating_point_v<typename C::value_type>, double>
    operator()(C const & c)
  {
    double tmp{0.0};
    for (auto i:c) tmp+=i*i;
    return std::sqrt(tmp);
  }

  //! For vector<complex<double>>
  template<class C>
  std::enable_if_t<TypeTraits::is_complex<typename C::value_type>::value, double>
    operator()(C const & c)
  {
    double tmp{0.0};
    // norm returns the square of the magnitude
    for (auto const & i:c) tmp+=std::norm(i);
    return std::sqrt(tmp);
  }
};

#endif
