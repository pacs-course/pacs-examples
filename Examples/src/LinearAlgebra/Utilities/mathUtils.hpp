/*
 * mathUtils.hpp
 *
 *  Created on: Dec 27, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_LINEARALGEBRA_UTILITIES_MATHUTILS_HPP_
#define EXAMPLES_SRC_LINEARALGEBRA_UTILITIES_MATHUTILS_HPP_
#include <type_traits>
#include <cmath>
#include <complex>
#include <numeric>
#include <functional>
#include <execution>
#include "is_complex.hpp"
#include "is_eigen.hpp"

namespace apsc
{
  namespace math_util
  {
    /*!
     * This utility is a callable object that performs a product of two complex numbers in the form
     * a * conj(b)
     * It degrades to the standard product if the arguments are not a complex.
     *
     * @tparam T the type
     */
    template <class T>
    struct product
    {
      /*!
       * The actual product operator
       * @param a the first argument
       * @param b The second argument
       * @return a* conj(b) if a and b are std::complex or just a*b
       */
      auto operator()(T const & a, T const & b)
      {
        if constexpr (apsc::TypeTraits::is_complex_v<T>)
            return a.real()*b.real() + a.imag()*b.imag();
        else
          return a*b;
      }
      /*!
       * Used to provide the result type.
       */
      //using result_type = decltype(std::declval<product<T>&>()(T(0), T(0)));
      //using result_type = decltype(this->operator()(T(0), T(0)));
  };

    /*!
     * A dot product utility that works uniformly for a scalar (then it is just the product),
     * complex numbers, Eigen vectors, containers storing scalars or complex numbers.
     *
     * @tparam T The type of the object to which the dot product applies
     * @param a First argument
     * @param b Second argument
     * @return The dot product, whose actual implementation depends on the type T
     */
    template <class T>
    auto dot(T const & a, T const & b)
    {
      if constexpr (apsc::TypeTraits::is_complex_v<T> || std::is_arithmetic_v<T>)
          return product<T>()(a,b);
      else if constexpr (apsc::TypeTraits::is_eigen_v<T>)
          return a.dot(b); // Eigen vectors have a dot member function
      else
        {
          using value_type=typename T::value_type;
          //using product_type=typename product<value_type>::result_type;
          using product_type=decltype(std::declval<product<value_type>&>()(value_type(0), value_type(0)));
            return std::transform_reduce(std::execution::par,
                                         std::begin(a),std::end(a),std::begin(b),
                                         static_cast<product_type>(0),
                                         std::plus<product_type>(),
                                       product<typename T::value_type>()
                                       );
        }
    }

    template <class T>
    auto norm2s(const T& a)
    {
      return dot(a,a);
    }

    template <class T>
    auto norm2(const T& a)
    {
      return std::sqrt(norm2s(a));
    }

    template<class T>
    auto normInf(const T& a)
    {
      if constexpr (apsc::TypeTraits::is_complex_v<T> || std::is_arithmetic_v<T>)
           return std::abs(a);
       else if constexpr (apsc::TypeTraits::is_eigen_v<T>)
           return a.template lpNorm<Eigen::Infinity>();
       else
         {
           using value_type=typename T::value_type;
           return std::abs(*std::max_element(std::begin(a),std::end(a),
                                   [](value_type const & x, value_type const & y){return std::abs(x)<std::abs(y);})
                           );
         }
    }

  }// end namepace math_util
}// end namespace apsc




#endif /* EXAMPLES_SRC_LINEARALGEBRA_UTILITIES_MATHUTILS_HPP_ */
