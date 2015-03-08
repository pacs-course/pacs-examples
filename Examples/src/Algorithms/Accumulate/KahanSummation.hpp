#ifndef HH_KAHANSUMMATION_HPP_
#define HH_KAHANSUMMATION_HPP_
#include <numeric>
#include <type_traits>
namespace Utility
{
  
  // Version for containers like vectors, array, sets. Does not work with maps
  template <class C,class Real=typename C::value_type>
  typename C::value_type kahanSummation(C const container)
  {
    static_assert(std::is_floating_point<Real>::value," kahanSummation makes sense only for containers of floating point values");
    long double runningError(0.);
    Real result(0.);
    for(auto const v : container)
      {
	long double y = v - runningError;
	long double t = result + y;
	runningError = (t - result) - y;
	result = t;
      }
    return result;
  }
  
} // namespace Utility

#endif // HH_KAHANSUMMATION_HPP_
