#ifndef HH_KAHANSUMMATION_HPP_
#define HH_KAHANSUMMATION_HPP_
#include <numeric>
#include <type_traits>
namespace Utility
{
  
  // Version for containers like vectors, array, sets. Does not work with maps
  template <class C,class Real=typename C::value_type>
  typename C::value_type kahanSummation(C const & container)
  {
    static_assert(std::is_floating_point<Real>::value," kahanSummation makes sense only for containers of floating point values");
    using Extended=std::conditional_t<std::is_same<Real,double>::value,long double, double>;

    //compensation for lost low-order bits
    Real runningError(0.0); 
    Real result(0.0);
    for(auto const v : container)
      {
	Extended y = v - runningError;
        // if result is big low order digits of y are lost
	Extended t = result + y;
        // (t-result) cancels the high order part of y
        // subtracting y recovers negative (low part of y)
	runningError = (t - result) - y;
        // result accumulates the sum
	result = t;
      }
    return static_cast<Real>(result);
  }
  
} // namespace Utility

#endif // HH_KAHANSUMMATION_HPP_
