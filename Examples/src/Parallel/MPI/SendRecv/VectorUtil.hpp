/*
 * VectorUtil.hpp
 *
 *  Created on: Aug 17, 2022
 *      Author: forma
 */

#ifndef AMSC_EXAMPLES_EXAMPLES_SRC_PARALLEL_MPI_SENDRECV_VECTORUTIL_HPP_
#define AMSC_EXAMPLES_EXAMPLES_SRC_PARALLEL_MPI_SENDRECV_VECTORUTIL_HPP_
#include <algorithm>
#include <array>
#include <random>
#include <vector>
#include <algorithm>
#ifdef CPP_PARALLEL
#include <execution> // for C++ native multithreading
#endif
#include "mpi.h"
namespace vectorUtil
{
/*!
 * Fills a vector with random numbers uniformly distributed in [-100,100)
 * @param v The vector
 */
inline void
fill_random(std::vector<double> &v)
{
  std::random_device                     rd{};
  std::mt19937                           gen{rd()};
  std::uniform_real_distribution<double> d{-100, 100};
  for(auto &x : v)
    x = d(gen);
}

/*!
 * Sorts elements vector in non-descending order. Used to implement the internal
 * operation in a odd-even transposition algorithm for parallel sorting.
 *
 * Additionally, it may implement multithreading using the parallel version of
 * the C++ sort algorithm. The multithreading parallel sort is activated only if
 * the code is compiled with -DCPP_PARALLEL and linked with -ltbb (for Intel
 * architectures)
 *
 * @param v In input the vector, in output the sorted vector
 */
inline void
sort_vec_int(std::vector<double> &v)
{
#ifdef CPP_PARALLEL
  std::sort(std::execution::par, std::begin(v), std::end(v));
#else
  std::sort(std::begin(v), std::end(v));
#endif
}

/*! Computes the parter of a rank in a odd-even transposition algorithm for
 * parallel sorting
 *
 * @param my_rank The current rank
 * @param comm_sz The number of ranks
 * @param phase the phase of the algorithm
 * @return the partner rank. MPI_PROC_NULL if my_rank is idle in this phase.
 */
constexpr int
my_partner(int const &my_rank, int const &comm_sz, int const &phase)
{
  int partner;
  if(phase % 2 == 0) // even phase
    {
      if(my_rank % 2 == 0) // even rank
        partner = my_rank + 1;
      else
        partner = my_rank - 1;
    }
  else
    {
      if(my_rank % 2 == 0) // even rank
        partner = my_rank - 1;
      else
        partner = my_rank + 1;
    }
  if(partner < 0 || partner >= comm_sz)
    partner = MPI_PROC_NULL;
  return partner;
}

/*!
 * A enum to select how to merge keys to be sorted
 */
enum MergeType
{
  LOW = 0,
  UP = 1
};

/*! A comparison operator to select low or up merge type
 *
 * @tparam Type The type of merge we want to implement
 * @param a Left key
 * @param b Right key
 * @return Comparison according to type
 */
template <MergeType Type>
constexpr auto
comp(double const &a, double const &b)
{
  if constexpr(Type == LOW)
    return (a <= b);
  else
    return (a >= b);
}

/*!
 * Advance a counter according to the type.
 * If we need to merge the greater elements we need to start from the end
 * and advance means retreating.
 * @tparam Type The type of merge we want to implement
 * @param i The index
 */
template <MergeType Type>
constexpr void
advance(std::size_t &i)
{
  if constexpr(Type == LOW)
    ++i;
  else
    --i;
}

/*!
 * Merges two ordered vector (from smallest to greatest) so that the returned
 * vector is similarly ordered and contains the smallest or greatest elements of
 * the two (according to the type).
 *
 * The result is of the same size of the first vector.
 * Example:
 *    my_keys=[1,2,7,10]
 *    partner_keys=[-2,3,4,90,100];
 *
 *    If Type =LOW result=[-2,2,3,4]
 *       Type =UP  result=[4,7,90,100]
 *
 * @tparam Type: UP or LOW if we want the greatest or the smallest
 * @param my_keys The first vector
 * @param partner_keys The second vector
 * @return A vector of the same size of my_keys with the selected keys.
 */
template <MergeType Type>
std::vector<double>
merge(std::vector<double> const &my_keys,
      std::vector<double> const &partner_keys)
{

  if(my_keys.size() == 0 or partner_keys.size() == 0)
    return my_keys; // a special case, do nothing

  std::vector<double> temp;
  temp.resize(my_keys.size());

  std::size_t m{0};
  std::size_t r{0};
  std::size_t t{0};
  std::size_t r_count{0};
  if constexpr(Type == UP)
    {
      m = my_keys.size() - 1u;
      r = partner_keys.size() - 1u;
      t = m;
    }

  for(auto i = 0u; i < my_keys.size(); ++i)
    {
      // the case we have exhausted partners keys
      if(r_count >= partner_keys.size())
        {
          // add remaining my keys
          temp[t] = my_keys[m];
          advance<Type>(m);
        }
      else if(comp<Type>(my_keys[m], partner_keys[r]))
        {
          // My key is better
          temp[t] = my_keys[m];
          advance<Type>(m);
        }
      else
        {
          // Partner key is better
          temp[t] = partner_keys[r];
          advance<Type>(r);
          ++r_count;
        }
      advance<Type>(t);
    }
  return temp;

}

} // namespace vectorUtil

#endif /* AMSC_EXAMPLES_EXAMPLES_SRC_PARALLEL_MPI_SENDRECV_VECTORUTIL_HPP_ */
