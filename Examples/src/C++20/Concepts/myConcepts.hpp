/*
 * myConcepts.hpp
 *
 *  Created on: Jan 13, 2022
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_C__20_CONCEPTS_MYCONCEPTS_HPP_
#define EXAMPLES_SRC_C__20_CONCEPTS_MYCONCEPTS_HPP_
#include <concepts>
#include <numeric>
#include <vector>
#include <cmath>
#include <memory>
#include "is_complex.hpp"
#include <vector>
#include <array>
#include <iostream>

namespace apsc::concepts
{
  /*!
     Using a user defined trait to define a simple concept
   */
  template <class T>
  concept complex = apsc::TypeTraits::is_complex_v<T>;

  /*!
   * @fn  complex_norm(const C&)
   * @brief A function to test a concept
   *
   * @tparam C a standard complex number type
   * @param x a complex number
   * @return The norm of x
   */
 template <complex C>
 constexpr  auto complex_norm(C const & x)
  {
    return std::sqrt(x.real()*x.real()+x.imag()*x.imag());
  }

  /* Alternative (less nice) syntax
  template <class C>
  requires complex<C>
   constexpr auto complex_norm(C const & x)
   {
     return std::sqrt(x.real()*x.real()+x.imag()*x.imag());
   }
 */

  /*!
    I define a concept of clonable requiring that
    the class contains a method called clone() that returns
    something convertible to a  unique pointer to the class.

    Note the use of a standard provided concept (convertible_to)
     and the syntax to indicate the return type.
    */
  template <typename T>
  concept clonable = requires( T v)
  {
    // the {} identify a non evaluating context
    {v.clone()}->std::convertible_to<std::unique_ptr<T>>;
  };
/*!
 * A function that takes clonable (using new simplifid syntax!)
 *
 * Alternative
 *
 * template<clonable T>
 * auto cloneMe(const T & c)
 */
inline auto cloneMe(const clonable auto & c)
{
  return c.clone();
}

/*!
 * You can also check for members!
 * Here I want that the class has a member called data that
 * is convertible to double
 */
template <typename T>
concept has_double_data = requires(T v)
{
  {v.data}->std::convertible_to<double>;
};

/*!
 * If we do not need to check return type of methods (or they are
 * returning void) the syntax is even simpler
 * Here I requireire that a StandardClock is a concept that contains
 * the methods start(), stop() and getTime();
 */
template <typename T>
concept StandardClock =
    requires(T c) {
    c.start();
    c.stop();
    c.getTime();
  };

/*!
 * A concept may invove also testing functions or operations
 * involving more than one argument
 *
 */
template<typename T>
concept summable = requires (T a, T b) { a + b; };

template<typename T>
concept multipliable = requires (T a, T b) { a * b; };

template <typename T>
concept init_by_zero = requires
{
  T{0};
};
/*
 * You may ask satisfaction of more than one
 */
template<typename T>
concept dottable = summable<T> and multipliable<T> and init_by_zero<T>;


/*!
 * Dot function for std::vectors
 */
template<dottable T>
inline auto dot(std::vector<T> const & a, std::vector<T> const & b)
{
  T res = T{0};
  for (unsigned int i=0;i<std::min(a.size(),b.size());++i)
    res+=a[i]*b[i];
return res;
}

/*!
 * If you want to test if a template dependent constant boolean expression
 * returns true, you need to us requires again otherwise the compiler checks only
 * if the expression is well formed, but NOT its value:
 */

/*!
 *  Concepts that tests if a std::array (or any other class with a
 *  constexpr method called size()) has size greater than 5.
 */
template <typename T>
concept smallerThan5 = requires(T v)
{
  /*!
   * size() must be a constexpr method since the result must be checked at compile time.
   * This is true for arrays. For vectors only under some conditions (since C++20), in general,
   * being a vector dynamically allocated, size() is not constexpr.
   * Remember that concepts test semantic of types, NOT of objects!
   */
  requires v.size()<=5;
 /*
  * if I omit the requires in the previous line the copiler only tests if
  * v has a method size(). The result of the comparison would be irrelevant.
  */
};

// Example of use in overload resolution. It can be done in a simpler way
// using if constexpr: it is just an example to show how overloading with concepts work.
// Indeed, this function does nothing a part writing up that it has been called.

//! Primary function
template <typename A>
void mul(A const & a){
  // Here the standard function
   std::cout<<"Calling the standard mul() function\n";
}
//! overloaded function
template <smallerThan5 A>
void mul(A const & a){
  // Here the special algorithm for small array
  std::cout<<"Calling the mul() function for short objects\n";
}





}// end namespace

#endif /* EXAMPLES_SRC_C__20_CONCEPTS_MYCONCEPTS_HPP_ */
