//
// Created by forma on 24/02/23.
//

#ifndef EXAMPLES_VECTORCONCEPT_H
#define EXAMPLES_VECTORCONCEPT_H
#if __cplusplus < 202002L
#error You must use at least c++20
#else
#include "is_complex.hpp"
#include <concepts>
/**
 * @namespace apsc
 * @brief Contains utilities and type traits for algebraic computations.
 *
 * The apsc namespace is designed to provide helper utilities and type traits
 * for linear algebra operations and concepts, facilitating generic programming
 * in numerical and scientific computing contexts.
 */
namespace apsc
{
namespace TypeTraits
{
  template <class T>
  /**
   * @concept AlgebraicVector
   * @brief Concept that defines the requirements for an algebraic vector type.
   *
   * This concept requires that a type `T` supports:
   * - Element access via the subscript operator (`v[0]`).
   * - Arithmetic operations on elements: addition (`+`), subtraction (`-`),
   * multiplication (`*`), and division (`/`).
   *
   * Types satisfying this concept can be used in generic algorithms that
   * operate on algebraic vectors.
   */
  concept AlgebraicVector = requires(T v) {
    { v[0] };
    { v[0] + v[0] };
    { v[0] / v[0] };
    { v[0] * v[0] };
    { v[0] - v[0] };
  };
} // namespace TypeTraits
} // namespace apsc
#endif

#endif // EXAMPLES_VECTORCONCEPT_H
