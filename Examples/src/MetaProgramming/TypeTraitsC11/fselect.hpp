#ifndef HH_FSELECT_HH__
#define HH_FSELECT_HH__
#include <type_traits>
namespace implementation
{
  //! Example of use of remove_pointer, true_type, false_type
  template <typename T>
    typename std::remove_pointer<T>::type inv_impl(const T val, std::true_type)
  {
    return 1./(*val);
  }
  
  template <typename T>
    T inv_impl(const T val, std::false_type)
  {
    return 1./(val);
  }
}
//! Changes return type and operation according to the type of the argument.
/*!
  Implementation is delegated to helper functions in the implementation namespace, using tag
  dispatch.
 */
template <typename T>
typename std::remove_pointer<T>::type inv(const T val){
  return implementation::inv_impl(val,std::is_pointer<T>());
}


#endif
