/*
 * hashCombine.hpp
 *
 *  Created on: Mar 15, 2022
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_UTILITIES_HASHCOMBINE_HPP_
#define EXAMPLES_SRC_UTILITIES_HASHCOMBINE_HPP_
#include <functional>
namespace apsc
{
/*!
 * @fn void hash_combine(std::size_t&, const T&, const Rest&...)
 * @brief To combine hash functions
 *
 * It help you to build a hash function combining values of
 * different types, provided that those types has a valid std::hash function
 *
 * Two versions are provided, one for c++17 onward using fold expressions,
 * and one for c++11/14 using a dummy array to expand the parameter pack.

 * @tparam T The first type
 * @tparam Rest the remaining types
 * @param seed a seed value that returns the final hash value (should be
 * initially set to 0)
 * @param v The first value
 * @param rest The remaining values.
 *
 * Possible use:
 *
 * @code
 * struct MyHash
 * {
 *   std::size_t operator()(MyClass const & x) const
 *   {
 *       std::size_t seed=0u;
 *       apsc::hash_combine(seed,x.firstmember,x.secondmember,x.thirdmember);
 *       return seed;
 *   }
 * };
 * @endcode
 */
#if __cplusplus >= 201703L
template <typename T, typename... Rest>
void
hash_combine(std::size_t &seed, const T &v, const Rest &...rest)
{
  std::hash<T> hasher;
  seed ^= hasher(v) + 0x9e3779b97f4a7c15 + (seed << 6) + (seed >> 2);
  (hash_combine(seed, rest), ...);
}
#else
/*!
 * @brief Version before c++17 (but you need at least c++11)
 *
 */
template <typename T, typename... Rest>
inline void
hash_combine(std::size_t &seed, T const &v, Rest const &...rest)
{
  std::hash<T> hasher;
  seed ^= hasher(v) + 0x9e3779b97f4a7c15 + (seed << 6) + (seed >> 2);
  // Trick to expand the parameter pack and call hashCombine for each argument
  // in C++11/14. The dummy array 'i' ensures that each call to hashCombine is
  // evaluated, and (void)(i) suppresses unused variable warnings.
  int i[] = {0, (hashCombine(seed, rest), 0)...};
  (void)(i);
}
#endif
} // namespace apsc

#endif /* EXAMPLES_SRC_UTILITIES_HASHCOMBINE_HPP_ */
