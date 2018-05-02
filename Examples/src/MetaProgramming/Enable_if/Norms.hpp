#include <cmath>
#include <type_traits>
#include <vector>
#include <complex>
#include <cmath>
#ifndef HH_NORMS_HH
#define HH_NORMS_HH
//! Works for vector<double> and vector<complex<double>
/*! 
  It can be made more general with some additional work.
  The same result can be obtained by specialization, but
  here if C is neither a vector<double> nor a vector<complex<double>>
  we have a compile time error, so it may be safer.
  Take it however just as an example of enable_if
 */
struct Norm2
{
  //! For vector<double>
  template<class C>
  typename std::enable_if<std::is_same<C,std::vector<double>>::value,
                          double>::type
    operator()(C const & c)
  {
    double tmp{0.0};
    for (auto i:c) tmp+=i*i;
    return sqrt(tmp);
  }
  //! For vector<double>
  template<class C>
  typename std::enable_if<std::is_same<C,std::vector<std::complex<double>> >::value,
                          double>::type
    operator()(C const & c)
  {
    double tmp{0.0};
    // norm returns the square of the magnitude
    for (auto const & i:c) tmp+=norm(i);
    return sqrt(tmp);
  }
};

#endif
