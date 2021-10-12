#ifndef __HH_MYTEML_HH__
#define __HH_MYTEML_HH__
//! @file mytemp.hpp
/*!
  @brief An example of template extern instantitiation.
  @note  It works only with C++11
 */
//! A class template
template <typename T> class Myclass
{
public:
  Myclass(T const &i) : my_data(i) {}
  double fun();

private:
  T my_data;
};

template <typename T>
double
Myclass<T>::fun()
{
  return 1. / my_data;
}

//! A function template
template <typename T>
double
func(T const &a)
{
  return 0.5 * a * a;
}

//! Extern explicit template instantiations
extern template class Myclass<double>;
extern template class Myclass<int>;
extern template double func<double>(double const &);
#endif
