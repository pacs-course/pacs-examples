#include "mytemp.hpp"
//! @file mytemp.cpp
/*!
  @brief explicit template instantiations.
  @detail With explicit instantiation we tell the compiler to generate the code
  for the template with the specified template arguments. For a class template
  all methods will be compiled.
 */
template class Myclass<double>;       // explicit instantiantion
template class Myclass<int>;          // explicit instantiation
template double func(double const &); // explicit instantiation
