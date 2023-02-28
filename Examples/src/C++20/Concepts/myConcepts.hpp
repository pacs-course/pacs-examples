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
#include <algorithm>

namespace apsc::concepts
{
  /*!
   * Creating a complex concept by combining existing ones form the concepts library
   *
   * In this case we define the concept of a type that is convertible to int and to
   * which the function std::swap(T,T) is defined.
   */
template <class T>
concept CtIandswappable = std::swappable<T> && std::convertible_to<T,int>;
  /*!
     Using a user defined trait to define a simple concept
     is_complex_v is defined in Utilities/is_complex.hpp
     apsc::TypeTraits::is_complex_v<T> is true if T is a std::complex<D>
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
    I define a concept of clonable requiring that the class contains a method called clone() that returns
    something convertible to a  unique pointer to the class.

    Note the use of a standard provided concept (convertible_to)
     and the syntax {}-> to indicate the return type.
    */
  template <typename T>
  concept clonable = requires( T v)
  {
    // the {} identify a non evaluating context
    {v.clone()}->std::convertible_to<std::unique_ptr<T>>;
  };
/*!
 * A function that takes clonable (using constrained auto syntax!)
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
 * Here I require that a StandardClock is a concept that contains
 * the methods start(), stop() and wallTimeNow();
 */
template <typename T>
concept StandardClock =
    requires(T c) {
    c.start();
    c.stop();
    c.wallTimeNow();
  };
/*!
 * A function tht measure the time taken to run a given function object double->double
 *
 * @tparam FUN the type of the function object
 * @tparam StandardClock a type satisfying the concept of a clock
 * @param value the value to be passed to the function
 * @return the time taken as the mean value of 1000 calls
 */
template<typename FUN, StandardClock C>
auto timeTaken(FUN const & f, double value,  C clock)
{
  clock.start();
  for(auto i = 0u; i < 1000; ++i)
    {
      [[maybe_unused]] auto y = f(value);
    }
  return clock.wallTimeNow()/1000.;
}
/*
 * A concept may involve also testing functions or operations
 * involving more than one argument
 *
 */
/*!
 * A concept testing if a type can be summed
 *
 * I use a requires expression to verify if the statement a+b is valid.
 * @tparam T The type to test
 */
template<typename T>
concept summable = requires (T a, T b) { a + b; };
/*!
 * A concept testing if a type can be divided
 *
 * I use a requires expression to verify if the statement a/b is valid
 * and returns a T. Note that the returned type cannot e simply T, It should be
 * something statically converted to true if the type is T
 *
 * @tparam T The type to test
 */
template<typename T>
concept divisible = requires (T a, T b) { {a/b}->std::same_as<T>; };
/*!
 * A concept testing if a type can be multiplied
 *
 * I use a requires expression to verify if the statement a*b is valid.
 * @tparam T The type to test
 */
 template<typename T>
concept multipliable = requires (T a, T b) { a * b; };
/*!
 * A concept testing if a type can be initialised by zero
 *
 * I use a requires expression to verify if the statement a*b is valid.
 * @tparam T The type to test
 */
template <typename T>
concept init_by_zero = requires
{
  T{0};
};


/*
 * You may ask satisfaction of more than one
 */
template<typename T>
concept field = summable<T> and multipliable<T> and divisible<T> and init_by_zero<T>;


/*!
 * Dot function for std::vectors
 */
template<class T>
requires summable<T> and multipliable<T> and init_by_zero<T>
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
double mul(A const & a){
  // Here the standard function
  double res=1.0;
  for (auto const & v:a) res*=v;
   std::cout<<"Calling the standard mul() function\n";
  return res;
}

//! overloaded function
template <smallerThan5 A>
double mul(A const & a){
  // Here the special algorithm for small array
  std::cout<<"Calling the mul() function for short arrays\n";
  switch(a.size())
    {
    case 0u: return 1.;
    case 1u: return a[0];
    case 2u: return a[0]*a[1];
    case 3u: return a[0]*a[1]*a[2];
    case 4u: return a[0]*a[1]*a[2]*a[3];
    default: return 1.;// only to avoid warning
    }
}





}// end namespace

#endif /* EXAMPLES_SRC_C__20_CONCEPTS_MYCONCEPTS_HPP_ */
