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

  In this case the SFINAE paradigm is activated on the return type of the
  function.
  
  \pre A callable object of type Norm2 works only if the argument is
  a vector<double> or vector<complex<double>>.
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
    return std::sqrt(tmp);
  }

  //! For vector<complex<double>>
  template<class C>
  typename std::enable_if<std::is_same<C,std::vector<std::complex<double>> >::value,
                          double>::type
    operator()(C const & c)
  {
    double tmp{0.0};
    // norm returns the square of the magnitude
    for (auto const & i:c) tmp+=std::norm(i);
    return std::sqrt(tmp);
  }
};

#endif
