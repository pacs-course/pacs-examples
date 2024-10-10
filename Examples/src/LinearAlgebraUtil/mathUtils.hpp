/*
 * mathUtils.hpp
 *
 *  Created on: Dec 27, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_LINEARALGEBRA_UTILITIES_MATHUTILS_HPP_
#define EXAMPLES_SRC_LINEARALGEBRA_UTILITIES_MATHUTILS_HPP_
#include "../../include/Arithmetic.hpp"
#include "Arithmetic.hpp"
#include "is_complex.hpp"
#include "is_eigen.hpp"
#include <cmath>
#include <complex>
#include <functional>
#include <iostream>
#include <numeric>
#include <ranges>
#include <type_traits>
#ifndef _OPENMP
// using C++ native multithreading
#include <execution>
#endif
namespace apsc
{
namespace math_util
{
  /*!
   * This utility is a callable object that performs a product of two complex
   * numbers in the form a * conj(b) It degrades to the standard product if the
   * arguments are not a complex.
   *
   * It is in an anonymous name space since it is not for general use
   *
   */
  namespace
  {
    template <class T>
      requires apsc::TypeTraits::ExtendedArithmetic<T>
    struct innerProduct
    {
      /*!
       * The actual product operator
       * @param a the first argument
       * @param b The second argument
       * @return a* conj(b) if a and b are std::complex or just a*b
       */
      auto
      operator()(T const &a, T const &b)
      {
        if constexpr(apsc::TypeTraits::is_complex_v<T>)
          return a.real() * b.real() + a.imag() * b.imag();
        else
          return a * b;
      }
    };
    /*!
     * Used to provide the result type.
     */
    // using result_type = decltype(std::declval<product<T>&>()(T(0), T(0)));
    // using result_type = decltype(this->operator()(T(0), T(0)));
  }; // namespace

  /*!
   * A dot product utility that works uniformly for a scalar (then it is just
   * the product), complex numbers, Eigen vectors, containers storing scalars or
   * complex numbers.
   *
   * @tparam T The type of the object to which the dot product applies
   * @param a First argument
   * @param b Second argument
   * @return The dot product, whose actual implementation depends on the type T
   */
  template <class T>
    requires apsc::TypeTraits::EigenMatrixType<T> ||
             apsc::TypeTraits::ExtendedArithmetic<T> ||
             std::ranges::random_access_range<T> decltype(auto)
  dot(T const &a, T const &b)
  {
    if constexpr(apsc::TypeTraits::is_complex_v<T> || std::is_arithmetic_v<T>)
      return innerProduct<T>{}(a, b);
    else if constexpr(apsc::TypeTraits::is_eigen_v<T>)
      return a.dot(b); // Eigen vectors have a dot member function
    else
      {
        using value_type = typename T::value_type;
        // using product_type=typename product<value_type>::result_type;
        // Here I use result_of instread of decltype (pre c++20 version)
        using product_type =
          std::result_of_t<innerProduct<value_type>(value_type, value_type)>;
        // using product_type = decltype(
        //     std::declval<innerProduct<value_type> >()(value_type(0),
        //     value_type(0)));
#ifndef _OPENMP
        return std::transform_reduce(
          std::execution::par, std::begin(a), std::end(a), std::begin(b),
          static_cast<product_type>(0), std::plus<product_type>(),
          innerProduct<value_type>());
#else
        product_type             res{0};
        innerProduct<value_type> prod;
#pragma omp parallel for reduction(+ : res)
        for(std::size_t i = 0u; i < a.size(); ++i)
          {
            res += prod(a[i], b[i]);
          }
        return res;
#endif
      }
  }

  /*!
   * Eigen matrices (and thus also Eigen vectors) have a method sqaredNorm that
   * returns the square of the norm. I need to have an adapter that provides an
   * interface usable also with standard vectors.
   *
   * The squaredNorm for all types for which I have defined dot
   * @tparam T The type stored in the vector
   * @param a the vector
   * @return the squared norm
   */
  template <typename T>
  decltype(auto)
  squaredNorm(T const &a)
  {
    return dot(a, a); // I have already defined the dot product!
  }

  /*!
   * @brief Specialization for Eigen
   *
   * Eigen matrices (and thus also vectors) have a method sqaredNorm that
   * returns the square of the norm. Here I need the corresponding function.
   * @tparam Derived Defines the type of the vector
   * @param a The vector
   * @return The square of the 2-norm of the vector
   */
  template <apsc::TypeTraits::EigenMatrixType T>
  decltype(auto)
  squaredNorm(const T &a)
  {
    return a.squaredNorm();
  }

  //! just an alternative name to SqaredNorm
  template <class T>
  decltype(auto)
  norm2s(const T &a)
  {
    return squaredNorm(a);
  }

  //! The usual norm2
  template <class T>
  decltype(auto)
  norm2(const T &a)
  {
    auto s = squaredNorm(a);
    return std::sqrt(s);
  }

  /*!
   * @brief Infinity norm
   *
   * @tparam T The type (arithmetic type, complex, std::vector, Eigen vector
   * @param a The datum
   * @return The infinity norm
   */
  template <class T>
    requires apsc::TypeTraits::EigenMatrixType<T> ||
             apsc::TypeTraits::ExtendedArithmetic<T> ||
             std::ranges::random_access_range<T> decltype(auto)
  normInf(const T &a)
  {
    if constexpr(apsc::TypeTraits::is_complex_v<T> || std::is_arithmetic_v<T>)
      return std::abs(a);
    else if constexpr(apsc::TypeTraits::is_eigen_v<T>)
      return a.template lpNorm<Eigen::Infinity>();
    else
      {
#ifndef _OPENMP
        using value_type = typename T::value_type;
        return std::abs(
          *std::max_element(std::execution::par, std::begin(a), std::end(a),
                            [](value_type const &x, value_type const &y) {
                              return std::abs(x) < std::abs(y);
                            }));
#else
        auto res = std::abs(a[0]);
#pragma omp parallel for reduction(max : res)
        for(std::size_t i = 0u; i < a.size(); ++i)
          res = std::max(std::abs(a[i]), res);
        return res;
#endif
      }
  }

  namespace vectorOperators
  {
    /*!
     * Addition for vectors
     * @tparam T The type contained in the vector
     * @param a
     * @param b
     * @return the addition
     */
    template <class T>
    std::vector<T>
    operator+(std::vector<T> const &a, std::vector<T> const &b)
    {
#ifndef _OPENMP
      std::vector<T> res(a.size(), 0); // a vector of zeros
      // I use std::transform because, by adding an execution policy I can make
      // it parallel! A note: the non parallel version can be made more
      // efficient... but then I cannot use std::transform
      std::transform(std::execution::par, a.begin(), a.end(), b.begin(),
                     res.begin(), std::plus<T>());
#else
      // a parallel version with openMP, more efficient probably
      std::vector<T> res = a;
#pragma omp parallel for
      for(std::size_t i = 0u; i < a.size(); ++i)
        res[i] += b[i]; // add-copy!
#endif
      return res;
    }

    template <typename T>
    std::vector<T>
    operator-(std::vector<T> const &a, std::vector<T> const &b)
    {
#ifndef _OPENMP
      std::vector<T> res(a.size(), 0);
      // I use std::transform because, by adding an execution policy I can make
      // it parallel! the sqme considerations for the + version apply here.
      std::transform(std::execution::par, a.begin(), a.end(), b.begin(),
                     res.begin(), std::minus<T>());
      // a parallel version with openMP, more efficient probably
#else
      std::vector<T> res = a;
#pragma omp parallel for
      for(std::size_t i = 0u; i < a.size(); ++i)
        res[i] -= b[i]; // subtract-copy!
#endif
      return res;
    }
    /*!
     * Pre-multiplication by a scalar. The scalar must of of the same type
     * contained in the vector
     * @note Beware! implicit conversion is NOT activated on template arguments.
     * The scalar must be of the same type of the vector's elements.
     * @tparam T The type contained in the vector
     * @param a A scalar
     * @param b A vector
     * @return a*b
     */
    template <typename T>
    std::vector<T>
    operator*(T const &a, std::vector<T> const &b)
    {
      std::vector<T> res = b;
#ifndef _OPENMP
      // I use std::transform because, by adding an execution policy I can make
      // it parallel!
      std::transform(std::execution::par, res.begin(), res.end(), res.begin(),
                     [&a](T const &y) { return a * y; });
      // version with a simple loop (it works fine as well)
#else
#pragma omp parallel for
      for(std::size_t i = 0u; i < res.size(); ++i)
        res[i] *= a;
#endif
      return res;
    }

    /*!
     * Post-multiplication by a scalar
     * Since I have a*b I want to have also b*a...
     * @tparam T The type contained in the vector
     * @param a A scalar
     * @param b A vector
     * @return b*a
     */
    template <typename T>
    std::vector<T>
    operator*(std::vector<T> const &b, T const &a)
    {
      return a * b; // no code duplication!
    }

    /*!
     * @brief Pretty printing for vectors
     *
     * @tparam T Vector element type
     * @param out output stream
     * @param v the vector
     * @return the aoutput stream
     */
    template <typename T>
    std::ostream &
    operator<<(std::ostream &out, std::vector<T> const &v)
    {
      out << "[";
      if(v.size() != 0)
        {
          std::copy(v.begin(), v.end() - 1, std::ostream_iterator<T>(out, " "));
          out << v.back();
        }
      out << "]";
      return out;
    }

  }; // namespace vectorOperators
  /*!
   * Often we need the squared distance of the point. I wrota an utility for the
   * purpose. Here the version for Eigen.
   * @tparam Derived The type of the first vector
   * @tparam OtherDerived The type of the second vector
   * @param a the first vector
   * @param b the second vector
   * @return ||a-b||^2
   */
  template <typename Derived, typename OtherDerived>
  decltype(auto)
  squaredDistance(const Eigen::MatrixBase<Derived>      &a,
                  Eigen::MatrixBase<OtherDerived> const &b)
  {
    return (a - b).squaredNorm();
  }

  template <typename T>
  auto
  squaredDistance(std::vector<T> const &a, std::vector<T> const &b)
  {
    using namespace vectorOperators;
    return squaredNorm(a - b);
  }

} // namespace math_util
} // end namespace apsc

#endif /* EXAMPLES_SRC_LINEARALGEBRA_UTILITIES_MATHUTILS_HPP_ */
