/*
 * HornerConstexpr.hpp
 *
 *  Created on: May 22, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_CONSTEXPR_HORNERCONSTEXPR_HPP_
#define EXAMPLES_SRC_CONSTEXPR_HORNERCONSTEXPR_HPP_
#include <type_traits>
/*! I need to have a template function so if T is a LiteralType (for instance a reference to an array)
 *  the constexpr mechanism activates.
 *  Moreover I want it to work also for complex
 */
template<class Coeff,class Value>
constexpr auto HornerConstexpr(Coeff const & coeff,Value const & x)
{
  // The type of u is the common type among the coefficient and the value x

  std::common_type_t<decltype(coeff[0]),Value> u = coeff.back();// back is a constexpr function for arrays
  // Since c++14 I can have for loops in constexpr functions
  // and size() is a constexpr function for arrays
    for (int  i=coeff.size()-2;i>=0;--i)
      u = u*x + coeff[i]; // [] operarator is constexpr for arrays
    return u;
}




#endif /* EXAMPLES_SRC_CONSTEXPR_HORNERCONSTEXPR_HPP_ */
