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
   * A class that tests if a type T is an Eigen matrix or vector
   *
   * @tparam T The type to test
   */
  template <class T, typename S=void> struct is_eigen : std::false_type
  {};

  /*!
    * A class that tests if a type T is an Eigen dense matrix or vector
    *
    * @tparam T The type to test
    */
  template <class T, typename S=void> struct is_dense_eigen : std::false_type
   {};

  /*!
     * A class that tests if a type T is an Eigen sparse matrix or vector
     *
     * @tparam T The type to test
     */
 template <class T, typename S=void> struct is_sparse_eigen : std::false_type
   {};

  /*!
   * Here a specialized class.
   * @tparam T The Derived type of the Eigen Matrix/Vector or Array
   */
  template <class T>
  struct is_eigen<T,
                  std::enable_if_t<
                    std::is_base_of_v<Eigen::MatrixBase<T>,T> ||
                    std::is_base_of_v<Eigen::SparseMatrixBase<T>,T> ||
                    std::is_base_of_v<Eigen::DenseBase<T>,T>
                    >
                  >
    : std::true_type
  {};

  /*!
   * Here a specialized class.
   * @tparam T The Derived type of the Sparse Eigen Matrix/Vector
   */
  template <class T>
  struct is_sparse_eigen<T,
                  std::enable_if_t<
                    std::is_base_of_v<Eigen::SparseMatrixBase<T>,T>
                    >
                  >
    : std::true_type
  {};
  /*!
   * Here a specialized class.
   * @tparam T The Derived type of the Dense Eigen Matrix/Array
   */
  template <class T>
   struct is_dense_eigen<T,
                   std::enable_if_t<
                   std::is_base_of_v<Eigen::MatrixBase<T>,T> ||
                   std::is_base_of_v<Eigen::DenseBase<T>,T>
                     >
                   >
     : std::true_type
   {};

  /*!
   * This inline constexpr mimick the utility present in standard type traits
   * since C++17
   */
  template <class T> inline bool constexpr is_eigen_v = is_eigen<T>::value;
  template <class T> inline bool constexpr is_sparse_eigen_v = is_sparse_eigen<T>::value;
  template <class T> inline bool constexpr is_dense_eigen_v = is_dense_eigen<T>::value;

#if  __cplusplus >= 202002L
  /*!
   * Concept for a generic Eigen Matrix/Array or expression
   */
  template <class T>
  concept EigenMatrixType = is_eigen_v<T>;
  /*!
   * Concept for a sparse Eigen matrix type or expression
   */
  template <class T>
  concept EigenSparseMatrixType = is_sparse_eigen_v<T>;
  /*!
   * Concept for a dense Eigen matrix/array type or expression
   */
  template <class T>
  concept EigenDenseMatrixType = is_dense_eigen_v<T>;
#endif

} // namespace TypeTraits

} // namespace apsc

#endif /* EXAMPLES_SRC_UTILITIES_IS_EIGEN_HPP_ */
