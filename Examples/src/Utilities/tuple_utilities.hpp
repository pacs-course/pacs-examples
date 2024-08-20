#ifndef HH_TUPLE_UTILITIES_HPP
#define HH_TUPLE_UTILITIES_HPP
// Inspired from the book "HPC C++"
#include <array>
#include <concepts>
#include <functional>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
namespace apsc
{
/*!
The common type of a tuple is the common type of its elements.
This is a recursive definition. So I start with an empty primary template
*/
template <typename Tuple> struct tuple_common_type
{
  using type = void;
};

/*!
The specialization for a tuple with at least one element
*/
template <typename... Types> struct tuple_common_type<std::tuple<Types...>>
{
  using type = std::common_type_t<Types...>;
};

/*!
A shortcut for the common type of the elements in a tuple

Usage:
@code
using myTuple = std::tuple<int, double, float>;
using myType = tuple_common_type_t<myTuple>;// mytype is double
@endcode
@param Tuple The tuple
*/
template <typename Tuple>
using tuple_common_type_t = typename tuple_common_type<Tuple>::type;

/*!

*/
/**
 * @brief Applies a given function to each element of a tuple.
 *
 * This function applies the provided function `f` to each element of the input
 * tuple `tuple`. The function `f` is invoked with each element of the tuple as
 * its argument. The results of each invocation are collected into a new tuple,
 * which is returned.
 *
 * @tparam Tuple The type of the input tuple.
 * @tparam F The type of the function object. Eache application of the function
 * should return a value (no void)!
 * @param tuple The input tuple.
 * @param f The function object to be applied to each element of the tuple.
 * @return A new tuple containing the results of applying the function to each
 * element of the input tuple.
 */
template <class Tuple, class F>
constexpr decltype(auto)
for_each(Tuple &&tuple, F &&f)
{
  return []<std::size_t... I>(Tuple && tuple, F && f, std::index_sequence<I...>)
  {
    return std::make_tuple(f(std::get<I>(tuple))...);
  }
  (std::forward<Tuple>(tuple), std::forward<F>(f),
   std::make_index_sequence<
     std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
}

/**
 * @brief Applies a given function to each element of a tuple.
 *
 * This function applies the provided function `f` to each element of the input
 * tuple `tuple`. The function `f` is invoked with each element of the tuple as
 * its argument. The function returns `f`, not the pruduces values. This
 * function is useful when you want to apply a function to a tuple but you are
 * not interested in the returned values. If you are interested in the returned
 * values use `for_each` instead.
 *
 * @tparam Tuple The type of the input tuple.
 * @tparam F The type of the function object. Eache application of the function
 * should return a value (no void)!
 * @param tuple The input tuple.
 * @param f The function object to be applied to each element of the tuple.
 * @return A copy of f
 */
template <class Tuple, class F>
constexpr decltype(auto)
for_each2(Tuple &&tuple, F &&f)
{
  return []<std::size_t... I>(Tuple && tuple, F && f, std::index_sequence<I...>)
  {
    (f(std::get<I>(tuple)), ...);
    return f;
  }
  (std::forward<Tuple>(tuple), std::forward<F>(f),
   std::make_index_sequence<
     std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
}

/**
 * @brief Checks if any element in a tuple satisfies a given predicate.
 *
 * This function iterates over the elements of a tuple and applies a given
 * predicate function to each element. It returns true if at least one element
 * satisfies the predicate, otherwise it returns false.
 *
 * @tparam Tuple The type of the tuple.
 * @tparam Func The type of the predicate function.
 * @tparam Index The index of the current element being checked (default is 0).
 * @param t The tuple to be checked.
 * @param f The predicate function to be applied to each element.
 * @return true if at least one element satisfies the predicate, false
 * otherwise.
 */
template <typename Tuple, typename Func, size_t Index = 0>
auto
tuple_any_of(const Tuple &t, const Func &f) -> bool
{
  constexpr auto n = std::tuple_size_v<Tuple>;
  if constexpr(Index < n)
    {
      bool success = std::invoke(f, std::get<Index>(t));
      if(success)
        {
          return true;
        }
      return tuple_any_of<Tuple, Func, Index + 1>(t, f);
    }
  else
    {
      return false;
    }
}

/**
 * @brief Checks if all elements in a tuple satisfy a given predicate.
 *
 * This function iterates over each element in the tuple and applies the given
 * predicate function to it. If all elements satisfy the predicate, the function
 * returns true; otherwise, it returns false.
 *
 * @tparam Tuple The type of the tuple.
 * @tparam Func The type of the predicate function.
 * @tparam Index The index of the current element being checked.
 * @param t The tuple to be checked.
 * @param f The predicate function to be applied to each element.
 * @return true if all elements satisfy the predicate, false otherwise.
 */
template <typename Tuple, typename Func, size_t Index = 0>
auto
tuple_all_of(const Tuple &t, const Func &f) -> bool
{
  constexpr auto n = std::tuple_size_v<Tuple>;
  if constexpr(Index < n)
    {
      bool success = std::invoke(f, std::get<Index>(t)) and
                     tuple_all_of<Tuple, Func, Index + 1>(t, f);
      return success;
    }
  else
    {
      return true;
    }
}

} // end namespace apsc
#endif