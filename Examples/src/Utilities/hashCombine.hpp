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
#if __cplusplus >= 201703L
  /*!
   * @fn void hash_combine(std::size_t&, const T&, const Rest&...)
   * @brief To combine hash functions
   *
   * It help you to build a hash function combining values of
   * different types, provided that thos types has a valid std::hash function
   *
   * This version uses fold expression and so it works only with c++17 onwards
   * @tparam T The first type
   * @tparam Rest the remaining types
   * @param seed a seed value that returns the final hash value (should be initially set to 0)
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
  template <typename T, typename... Rest>
  void hash_combine(std::size_t& seed, const T& v, const Rest&... rest)
  {
      std::hash<T> hasher;
      seed ^= hasher(v) + 0x9e3779b97f4a7c15 + (seed << 6) + (seed >> 2);
      (hash_combine(seed,rest), ...);
  }
#else
  /*!
   * @brief Version before c++17 (but you need at least c++11)
   *
   */
  template <typename T, typename... Rest>
  inline void hashCombine(std::size_t &seed, T const &v, Rest const &... rest) {
      std::hash<T> hasher;
      seed ^= hasher(v) + 0x9e3779b97f4a7c15 + (seed << 6) + (seed >> 2);
      int[] i={0, (hashCombine(seed, rest), 0)...};
      //!@note: the following line is to avoid the warning of unused variable i
      (void)(i);
  }
#endif
}



#endif /* EXAMPLES_SRC_UTILITIES_HASHCOMBINE_HPP_ */
