#pragma once
#include <tuple>
#include <utility>

namespace apsc
{
// version before c++20
#if __cplusplus < 202002L
namespace detail
{
  /*!
  @brief Applies a callable object to each element of a tuple
  @details This function is a C++17 version of the for_each function. It uses
  index sequences to iterate over the elements of the tuple. The fold expression
  is used to call the function for each element of the tuple.
  @note The function returns the function object after applying it to all
  elements of the tuple. This allows for chaining of function calls if the
  function object is designed to return itself after being called.
  @pre The function object must be callable with the types of the elements
  @tparam Tuple The type of the tuple
  @tparam F The type of the function object
  @tparam I The indices of the tuple elements
  @param tuple The tuple to iterate over
  @param f The function to apply
  @return The result of the function application
  */
  template <class Tuple, class F, std::size_t... I>
  F
  for_each_impl(Tuple &&tuple, F &&f, std::index_sequence<I...>)
  {
    // Expand the index pack I... into get<0>(tuple), get<1>(tuple), ...
    // and call f on each element through a fold expression over the comma
    // operator.
    (f(std::get<I>(tuple)), ...);
    // Return the callable so stateful function objects can be inspected after
    // the traversal.
    return f;
  }
} // namespace detail
/*! @brief Applies a callable object to each element of a tuple
 * @details This function is a C++17 version of the for_each function. It uses
 * index sequences to iterate over the elements of the tuple. The fold
 * expression is used to call the function for each element of the tuple.
 * @note The function returns the function object after applying it to all
 * elements of the tuple. This allows for chaining of function calls if the
 * function object is designed to return itself after being called.
 * @pre The function object must be callable with the types of the elements
 * of the tuple. The tuple must be a std::tuple or a type that can be used with
 * std::get and std::index_sequence.
 * @tparam Tuple The type of the tuple
 * @tparam F The type of the function object
 * @param tuple The tuple to iterate over
 * @param f The function to apply
 * @return The result of the function application
 */
template <class Tuple, class F>
constexpr decltype(auto)
for_each(Tuple &&tuple, F &&f)
{
  // Build the sequence 0, 1, ..., N-1 where N is the tuple size, then
  // delegate the actual expansion to the implementation helper.
  return detail::for_each_impl(
    std::forward<Tuple>(tuple), std::forward<F>(f),
    std::make_index_sequence<
      std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
}

#else
/*! @brief Applies a callable object to each element of a tuple
 * @details This function is a C++20 version of the for_each function. It uses
 std::apply to apply the function to each element of the tuple. The fold
 expression is used to call the function for each element of the tuple.
 * @note The function returns the function object after applying it to all
 elements of the tuple. This allows for chaining of function calls if the
 function object is designed to return itself after being called.
  * @pre The function object must be callable with the types of the elements
 of the tuple. The tuple must be a std::tuple or a type that can be used with
 std::apply.
 * @tparam Tuple The type of the tuple
 * @tparam F The type of the function object
 * @param tuple The tuple to iterate over
 * @param f The function to apply
 * @return The result of the function application
 */
template <class Tuple, class F>
constexpr decltype(auto)
for_each(Tuple &&tuple, F &&f)
{
  // std::apply unpacks the tuple into the lambda parameters args...
  std::apply(
    // Forward each unpacked element to f and evaluate the calls left to right.
    [&f](auto &&...args) { (f(std::forward<decltype(args)>(args)), ...); },
    std::forward<Tuple>(tuple));
  // Returning f mirrors the C++17 version and preserves any state accumulated
  // inside the callable.
  return f;
}
#endif
} // namespace apsc
