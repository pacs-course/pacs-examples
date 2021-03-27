/*
 * hashTuple.hpp
 *
 *  A hash function for the hash
 *  inspired from boost:hash_tuple
 *
 *  Created on: Mar 5, 2021
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_STL_TUPLE_HASHTUPLE_HPP_
#define EXAMPLES_SRC_STL_TUPLE_HASHTUPLE_HPP_
#include <tuple>
#include <functional>
#include <type_traits>


namespace apsc::Utility
{

  namespace details
  {
    template <typename SizeT>
    inline void hash_combine_impl(SizeT& seed, SizeT value) noexcept
    {
      seed ^= value + 0x9e3779b9 + (seed<<6) + (seed>>2);
    }

    template <class T>
    inline void hash_combine(std::size_t& seed, T const& v) noexcept
    {
      std::hash<T> hasher;
      hash_combine_impl(seed, hasher(v));
    }

    template <std::size_t I, typename T>
    inline std::enable_if_t<(I == std::tuple_size<T>::value)>
    hash_combine_tuple(std::size_t&, T const&) noexcept
    { }

    template <std::size_t I, typename T>
    inline std::enable_if_t<(I < std::tuple_size<T>::value)>
    hash_combine_tuple(std::size_t& seed, T const& v) noexcept
    {
      hash_combine(seed, std::get<I>(v));
      hash_combine_tuple<I + 1>(seed, v);
    }
  }

  template <typename T>
  struct HashTuple
  {
    std::size_t operator()(T const& v) const noexcept
    {
      std::size_t seed = 0;
      details::hash_combine_tuple<0>(seed, v);
      return seed;
    }
  };
}


#endif /* EXAMPLES_SRC_STL_TUPLE_HASHTUPLE_HPP_ */
