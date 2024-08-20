#ifndef HH_PARALLEL_FOR_HPP_HH
#define HH_PARALLEL_FOR_HPP_HH
#include <algorithm>
#include <concepts>
#include <execution>
#include <ranges>
namespace apsc
{
/**
 * @brief Executes a parallel loop over a range of indices using the specified
 * execution policy.
 *
 * This function executes a parallel loop over a range of indices specified by
 * `first` and `last`, using the specified execution policy `p`. The loop body
 * is defined by the callable object `f`, which takes an index as its argument.
 *
 * @tparam Policy The type of the execution policy.
 * @tparam Index The type of the loop indices.
 * @tparam F The type of the callable object representing the loop body.
 *
 * @param p The execution policy to use for parallel execution.
 * @param first The first index of the loop range (inclusive).
 * @param last The last index of the loop range (exclusive).
 * @param f The callable object representing the loop body.
 */
template <typename Policy, std::integral Index, typename F>
  requires requires(Policy p, Index i, F f) {
             f(i);
             requires std::is_execution_policy_v<std::remove_cvref_t<Policy>>;
           }
auto
parallel_for(Policy &&p, Index first, Index last, F f) -> void
{
  auto r = std::ranges::views::iota(first, last);
  std::for_each(p, r.begin(), r.end(), std::move(f));
}
} // namespace apsc
#endif