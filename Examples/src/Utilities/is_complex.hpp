#ifndef HH_IS_COMPLEX_HHH
#define HH_IS_COMPLEX_HHH
#include <complex>
#include <type_traits>
#if  __cplusplus >= 202002L
#include <concepts>
#endif
/*!
 * @file is_complex.hpp
 * Apparently the Standard Library has no trait to test if a type is a
 * complex<T> number This is a simple implementation with just one defect. It
 * tests if the type is a
 *
 */
namespace apsc
{
namespace TypeTraits
{
  //! Primary template: false
  template <class T> struct is_complex : public std::false_type
  {
    /*!
     * The type of the complex. Here set to void
     */
    using complex_type = void;
  };

  //! Specialization: true if complex
  template <class T> struct is_complex<std::complex<T>> : public std::true_type
  {
    /*!
     * The type of the complex
     */
    using complex_type = std::complex<T>;
  };

  //! to be consistent with C++17 style
  template <class T> inline bool constexpr is_complex_v = is_complex<T>::value;
  template <class T> using is_complex_t = typename is_complex<T>::type;

#if  __cplusplus >= 202002L
  template <class T>
  concept Complex = is_complex_v<T>;// I have the trait so I can just do this
  /*concept Complex = requires(T a)
  {
  typename T::value_type;
  {a.imag()}->std::same_as<typename T::value_type>;
  {a.real()}->std::same_as<typename T::value_type>;
  };*/
  /*!
   * A concept expressing a complex number on the float field
   */
  template<class T>
  concept FloatComplex=
  requires(T x)
  {
      typename T::value_type;
      requires Complex<T> && std::floating_point<typename T::value_type>;
  };

  /*!
   * Concept including FloatComplex and complex<T> with T an integral type
   */
  template<class T>
   concept ArithmeticComplex= Complex<T> ||
   requires(T x)
   {
       typename T::value_type;
       requires std::integral<typename T::value_type>;
   };


#endif
} // end namespace TypeTraits
} // end namespace apsc
#endif
