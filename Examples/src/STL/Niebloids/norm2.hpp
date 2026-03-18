#ifndef STL_NIEBLOIDS_NORM2_HPP
#define STL_NIEBLOIDS_NORM2_HPP

#include <cmath>
#include <concepts>
#include <functional>
#include <ranges>
#include <utility>

namespace apsc::niebloids
{
/*!
 * @brief A function object to compute the 2-norm of a vector
 */
struct norm2_fn
{
  /*!
   * @brief Computes the 2-norm of any iterable range of elements
   *
   * @tparam T, the range type that must satisfy the std::ranges::range concept
   * @param s a range representing the containtr to iterate over.
   *  It can be a std::vector, a std::array, or any other range that satisfies
   * the concept.
   * @return the 2-norm of the vector
   *
   * This function computes the 2-norm of a vector by summing the squares of its
   * elements and taking the square root of the sum. It accepts any range that
   * satisfies the std::ranges::range concept, making it flexible and adaptable
   * to different types of containers. The use of concepts ensures that the
   * function can only be instantiated with types that meet the required
   * criteria, providing compile-time safety. The function is designed to be
   * efficient and easy to use.
   */
  template <std::ranges::range T>
  auto
  operator()(T s) const
  {
    // sum is of the type contained in the range
    using value_type = std::ranges::range_value_t<T>;
    value_type sum{};
    for(const auto &value : s)
      sum += value * value;
    return std::sqrt(sum);
  }
};
/*!
 * @brief An instance of the norm2_fn function object
 *
 * This is called a niebliod. It is a function object that can be used like a
 * regular function. The inline constexpr specifier allows it to be defined in a
 * header file without violating the One Definition Rule, and it can be used in
 * constant expressions.
 */
inline constexpr norm2_fn norm2{};
} // namespace apsc::niebloids

#endif
