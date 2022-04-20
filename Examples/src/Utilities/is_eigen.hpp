/*
 * is_eigen.hpp
 *
 *  Created on: Dec 27, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_UTILITIES_IS_EIGEN_HPP_
#define EXAMPLES_SRC_UTILITIES_IS_EIGEN_HPP_
#include "Eigen/Core"
#include "Eigen/SparseCore"
#include <type_traits>
namespace apsc
{
namespace TypeTraits
{
  /*!
   * A class that tests if a type T derives from Eigen::MatrixBase<T> or
   * Eigen::SparseMatrixBase<T>
   * It can be used to verify if a type is an Eigen matrix or vector
   *
   * @tparam T The type to test
   */
  template <class T, typename S=void> struct is_eigen : std::false_type
  {};

  /*!
   * Here the specialised function.
   * @tparam T The Derived type of the Eigen Matrix/Vector
   */
  template <class T>
  struct is_eigen<T,
                  std::enable_if_t<
                    std::is_base_of_v<Eigen::MatrixBase<T>,T>
                    >
                  >
    : std::true_type
  {};

  /*!
   */
  template <class T>
  struct is_eigen<T,
                  std::enable_if_t<
                    std::is_base_of_v<Eigen::SparseMatrixBase<T>,T>
                    >
                  >
    : std::true_type
  {};

  /*!
   * This inline constexpr mimick the utility present in standard type traits
   * since C++17
   */
  template <class T> inline bool constexpr is_eigen_v = is_eigen<T>::value;
} // namespace TypeTraits

} // namespace apsc

#endif /* EXAMPLES_SRC_UTILITIES_IS_EIGEN_HPP_ */
