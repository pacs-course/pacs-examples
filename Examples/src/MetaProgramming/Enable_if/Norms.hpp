#include <type_traits>
#include <vector>
#include <complex>
#include <cmath>
#ifndef HH_NORMS_HH
#define HH_NORMS_HH
//! Computes the 2-norm of a std:;vector<double> or of a std::vector<std::complex<double>>
/*! 
  Is  an example of the use of enable_if to select a specific function member depending on the type
  of the template argument.

  In this case the SFINAE paradigm is activated on the return type of the call operator.

  <b>Take it only as an example</b>. The function std::norm works also for floating point types, so you do not
  need to have two separate call operators, you might just use the second version (taking away the enable_if suff of course)
  
  \note The class template Norm2 works only if the template argument is
  a vector<double> or vector<complex<double>>. Making it more general is possible, but more involved.
 */
struct Norm2
{
  //! For vector<double>
  template<class C>
  std::enable_if_t<std::is_same_v<C,std::vector<double>>, double>
    operator()(C const & c)
  {
    double tmp{0.0};
    for (auto i:c) tmp+=i*i;
    return std::sqrt(tmp);
  }

  //! For vector<complex<double>>
  template<class C>
  std::enable_if_t<std::is_same_v<C,std::vector<std::complex<double>> >, double>
    operator()(C const & c)
  {
    double tmp{0.0};
    // norm returns the square of the magnitude
    for (auto const & i:c) tmp+=std::norm(i);
    return std::sqrt(tmp);
  }
};

#endif
