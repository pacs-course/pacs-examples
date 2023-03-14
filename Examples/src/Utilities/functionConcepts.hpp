/*
 * functionConcepts.hpp
 *
 *  Created on: Mar 14, 2023
 *      Author: forma
 */

#ifndef UTILITIES_FUNCTIONCONCEPTS_HPP_
#define UTILITIES_FUNCTIONCONCEPTS_HPP_
#include <functional>
#include <concepts>
#include <vector>
#include <array>
namespace apsc::TypeTraits
{
  template<class F>
  concept ScalarFunction=std::convertible_to<F,std::function<double (double)> > ||
  std::convertible_to<F,std::function<double (double const &)> >;

  template<class F>
  concept RnToRnFunction=
      std::convertible_to<F,std::function<std::vector<double> (std::vector<double> const &)> >;

  template<class F>
   concept RnToRFunction=
       std::convertible_to<F,std::function<double (std::vector<double> const &)> >;
}





#endif /* UTILITIES_FUNCTIONCONCEPTS_HPP_ */
