/*
 * generalPow.hpp
 *
 *  Created on: May 23, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_METAPROGRAMMING_POW_GENERALPOW_HPP_
#define EXAMPLES_SRC_METAPROGRAMMING_POW_GENERALPOW_HPP_
#include "Pow.hpp"
#include <cmath>
#include <type_traits>
namespace apsc
{
  //! enabled if 2nd argument is an unsigned! integral type (unsigned int, unsigned long int...)
  template<class R, class EXP>
  constexpr typename std::enable_if<std::is_integral<EXP>::value && std::is_unsigned<EXP>::value,R >::type
  pow(R const & x, EXP const n)
  {
    return apsc::POW(x,n);
  }

  //! if the exponent is a signed int type I cannot know if the argument is positive or not so I do a trick
  template<class R, class EXP>
   constexpr typename std::enable_if<std::is_integral<EXP>::value && std::is_signed<EXP>::value,R >::type
   pow(R const & x, EXP const n)
   {
    if (n<0)
     return R(1.0)/apsc::POW(x,static_cast<typename std::make_signed<EXP>::type> (-n));
    else
     return apsc::POW(x,static_cast<typename std::make_signed<EXP>::type> (n));
   }


  template<class R, class EXP>
   typename std::enable_if<std::is_floating_point<EXP>::value,R >::type
   pow(R const & x, EXP const n)
   {
     return std::pow(x,n);
   }

}




#endif /* EXAMPLES_SRC_METAPROGRAMMING_POW_GENERALPOW_HPP_ */
