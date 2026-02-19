/*
 * mathUtils.hpp
 *
 *  Created on: Dec 27, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_LINEARALGEBRA_UTILITIES_MATHUTILS_HPP_
#define EXAMPLES_SRC_LINEARALGEBRA_UTILITIES_MATHUTILS_HPP_
#include "Arithmetic.hpp"
#include "is_complex.hpp"
#include "is_eigen.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <complex>
#include <cstddef>
#include <execution>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <type_traits>
#include <vector>
namespace apsc
{
namespace math_util
{
  /*!
   * This utility is a callable object that performs a product of two complex
   * numbers in the form a * conj(b) It degrades to the standard product if the
   * arguments are not a complex.
   *
   * It is in a detail namespace since it is not for general use
   *
   */
  namespace detail
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
      constexpr auto
      operator()(T const &a, T const &b) const
      {
        if constexpr(apsc::TypeTraits::is_complex_v<T>)
          return a * std::conj(b);
        else
          return a * b;
      }
    };
  } // namespace detail

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
    requires apsc::TypeTraits::EigenMatrixType<T> ||    // ok if it is an eigen
                                                        // vector
             apsc::TypeTraits::ExtendedArithmetic<T> || // ok if it is a scalar
                                                        // or a complex
             (std::ranges::random_access_range<T> &&    // ok if it is a random
                                                     // access range and I can
                                                     // get its size
              std::ranges::sized_range<T>)
  decltype(auto)
  dot(T const &a, T const &b)
  {
    if constexpr(apsc::TypeTraits::is_complex_v<T> || std::is_arithmetic_v<T>)
      return detail::innerProduct<T>{}(
        a, b); // for scalars and complex numbers I just use the inner product
               // defined in the detail namespace
    else if constexpr(apsc::TypeTraits::is_eigen_v<T>)
      return a.dot(b); // Eigen vectors have a dot member function
    else
      {
        using value_type =
          std::ranges::range_value_t<T>; // the type of the elements stored in
                                         // the vector
        using product_type =
          std::invoke_result_t<detail::innerProduct<value_type>,
                               value_type, // the type of the result of the
                                           // inner product of two elements of
                                           // the vector
                               value_type>;
        if(std::ranges::empty(a))
          return product_type{}; // if the vector is empty I return the default
                                 // value of the product type, which is 0 for
                                 // arithmetic types and 0+0i for complex
                                 // numbers
        assert(std::ranges::size(a) ==
               std::ranges::size(b)); // the vectors must have the same size
#ifndef _OPENMP
        // I use std::transform_reduce because, by adding an execution policy I
        // can make it parallel!
        return std::transform_reduce(
          std::execution::par, std::ranges::begin(a), std::ranges::end(a),
          std::ranges::begin(b),
          static_cast<product_type>(0), // the initial value of the reduction,
                                        // note the cast to product_type
          std::plus<product_type>(), detail::innerProduct<value_type>());
#else
        product_type                     res = 0;
        detail::innerProduct<value_type> prod;

// OpenMP reduction does not support user-defined types like std::complex,
// so we need to do a manual reduction for such types.
#pragma omp parallel
        {
          product_type local_res = 0;
#pragma omp for nowait
          for(std::size_t i = 0u; i < a.size(); ++i)
            {
              local_res += prod(a[i], b[i]);
            }
#pragma omp critical
          {
            res += local_res;
          }
        }
        return res;
#endif
      }
  }

  /*!
   * Eigen matrices (and thus also Eigen vectors) have a method squaredNorm that
   * returns the square of the norm. I need to have an adapter that provides an
   * interface usable also with standard vectors.
   *
   * The squaredNorm for all types for which I have defined dot
   * @tparam T The type stored in the vector
   * @param a the vector
   * @return the squared norm
   */
  template <typename T>
  [[nodiscard]] decltype(auto)
  squaredNorm(T const &a)
  {
    return dot(a, a); // I have already defined the dot product!
  }

  /*!
   * @brief Specialization for Eigen
   *
   * Eigen matrices (and thus also vectors) have a method squaredNorm that
   * returns the square of the norm. Here I need the corresponding function.
   * @tparam Derived Defines the type of the vector
   * @param a The vector
   * @return The square of the 2-norm of the vector
   */
  template <apsc::TypeTraits::EigenMatrixType T>
  [[nodiscard]] decltype(auto)
  squaredNorm(const T &a)
  {
    return a.squaredNorm();
  }

  //! just an alternative name to squaredNorm
  template <class T>
  [[nodiscard]] decltype(auto)
  norm2s(const T &a)
  {
    return squaredNorm(a);
  }

  //! The usual norm2
  template <class T>
  [[nodiscard]] decltype(auto)
  norm2(const T &a)
  {
    auto s = squaredNorm(a);
    return std::sqrt(std::abs(s));
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
             (std::ranges::random_access_range<T> &&
              std::ranges::sized_range<T>)
  [[nodiscard]] decltype(auto)
  normInf(const T &a)
  {
    if constexpr(apsc::TypeTraits::is_complex_v<T> || std::is_arithmetic_v<T>)
      return std::abs(a);
    else if constexpr(apsc::TypeTraits::is_eigen_v<T>)
      return a.template lpNorm<Eigen::Infinity>();
    else
      {
        using value_type = std::ranges::range_value_t<T>;
        using abs_type = decltype(std::abs(std::declval<value_type>()));
        if(std::ranges::empty(a))
          return abs_type{};
#ifndef _OPENMP
        return std::transform_reduce(
          std::execution::par, std::ranges::begin(a), std::ranges::end(a),
          abs_type{}, [](abs_type x, abs_type y) { return std::max(x, y); },
          [](value_type const &x) { return std::abs(x); });
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
      assert(a.size() == b.size());
#ifndef _OPENMP
      std::vector<T> res(a.size(),
                         T{}); // a vector of default-initialized elements
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
      assert(a.size() == b.size());
#ifndef _OPENMP
      std::vector<T> res(a.size(), T{});
      // I use std::transform because, by adding an execution policy I can make
      // it parallel! The same considerations for the + version apply here.
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
     * @return the output stream
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
   * Often we need the squared distance of a point. I wrote a utility for the
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
    assert(a.size() == b.size());
    return squaredNorm(vectorOperators::operator-(a, b));
  }

} // namespace math_util
} // end namespace apsc

#endif /* EXAMPLES_SRC_LINEARALGEBRA_UTILITIES_MATHUTILS_HPP_ */
