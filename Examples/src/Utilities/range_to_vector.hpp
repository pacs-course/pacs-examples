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
 * Unfortunately c++ has not yet utilities to convert a range to a vector (or
 * other containers) Here we provide a simple implementation
 * @note More general implementation may be found in
 * https://timur.audio/how-to-make-a-container-from-a-c20-range
 * @note Since c++23 we have std::ranges::to<T> to convert a view range into a
 * non-view range (a container), so this utility is obsolete.
 * @param r The range
 * @return A vector containing the elements of the range
 */
auto
ranges_to_vector(std::ranges::range auto &&r)
{
  // A note: you can replace auto&& above with const auto &.
  // auto&& creates a forwarding reference, but no need of it here!
  // Anyway, if you prefer auto&& it is fine.
  // Here I use std::ranges::range_value_t to extact the type
  // of the elements of the range.
  std::vector<std::ranges::range_value_t<decltype(r)>> v;
  // If the range has the method size() we reserve the memory to speed up the
  // copy. Again I usa another c++20 feature: std::ranges::sized_range
  if constexpr(std::ranges::sized_range<decltype(r)>)
    {
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
