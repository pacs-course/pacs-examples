/*
 * LambdaDerivatives.hpp
 *
 *  Created on: Jan 8, 2022
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_DERIVATIVES_LAMBDADERIVATIVES_HPP_
#define EXAMPLES_SRC_DERIVATIVES_LAMBDADERIVATIVES_HPP_
namespace apsc
{
  /*!
   * @namespace lambda
   * @brief To avoid name clashes I put this utility under a sub-namespace
   *
   */
  namespace lambda
  {
    /*!
     * A lambda recursive function!
     *
     * Example of use:
     *
     * f = auto [](double x){return std::sin(x);}
     * auto d = lambda::derive<4>(f,3.2,1.e-4);
     *
     * Taken from Discovering Modern C++, II Edition, by Peter Gottschling
     *
     */
  template<unsigned int N>
  auto derive =[](auto f, auto x, auto h){
   if constexpr (N==0u)
    {
       return f(x);
    }
   else
     {
       auto prev = [&](auto x2)
           {
         return derive<N-1u>(f, x2,h);
           };
    return (prev(x+h)-prev(x-h))/(2.*h);
     }

  };
  }
}




#endif /* EXAMPLES_SRC_DERIVATIVES_LAMBDADERIVATIVES_HPP_ */
