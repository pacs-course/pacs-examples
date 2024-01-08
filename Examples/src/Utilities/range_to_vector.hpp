#ifndef EXAMPLES_RANGE_TO_VECTOR_HPP
#define EXAMPLES_RANGE_TO_VECTOR_HPP
#include <vector>
#if __cplusplus >= 202002L
#include <ranges>
namespace Utilities
{
/*!
 * @brief A function that converts a range or a view to a vector
 *
 * Unfortunately c++ has not yet utilities to convert a range to a vector (or other containers)
 * Here we provide a simple implementation
 * @note More general implementation may be found in https://timur.audio/how-to-make-a-container-from-a-c20-range
 * @param r The range
 * @return A vector containing the elements of the range
 */
auto ranges_to_vector(std::ranges::range auto&& r) {
  std::vector<std::ranges::range_value_t<decltype(r)>> v;
  // If the range has the method size() we reserve the memory to speed up the copy
  if constexpr(std::ranges::sized_range<decltype(r)>) {
    v.reserve(std::ranges::size(r));
  }
  std::ranges::copy(r, std::back_inserter(v));
  return v;
}
} // end namespace Utilities
#else
#message "This file requires at least c++20"
#abort
#endif

#endif
