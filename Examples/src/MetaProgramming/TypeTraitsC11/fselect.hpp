#ifndef HH_FSELECT_HH__
#define HH_FSELECT_HH__
#include <type_traits>
template <typename T>
typename std::remove_pointer<T>::type inv_impl(const T val, std::true_type)
{
  return 1./(*val);
}

template <typename T>
typename std::remove_pointer<T>::type inv_impl(const T val, std::false_type)
{
  return 1./(val);
}

template <typename T>
typename std::remove_pointer<T>::type inv(const T val){
  return inv_impl(val,std::is_pointer<T>());
}


#endif
