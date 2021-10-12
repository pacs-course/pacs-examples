/*
 * sampling.hpp
 *
 *  Created on: Apr 5, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_RANDOMDISTIBUTIONS_SAMPLING_HPP_
#define EXAMPLES_SRC_RANDOMDISTIBUTIONS_SAMPLING_HPP_
#include <algorithm>
#include <random>
namespace apsc
{
//! An example of use of random sampling
//! It returns a different sample of max n elements extracted uniformily
//! from a given container.
//! @tparam Cont  Any standard container.
//! @param in The container from which extract the sample.
//! @param n Number of samples.
//! @return The sampled elements.If n greater than the container size the whole
//! input container is returned
template <class Cont>
Cont
sampleContainer(const Cont &in, unsigned int n)
{
  Cont res;
  if(n != 0)
    {
      if(n >= in.size())
        {
          res = in; // returns everything
        }
      else
        {
          // I am using the random device to initialize a Marsenne type
          // random number generator. I have to use inserter() and not
          // back_inserter, since it is uniform across containers. This form of
          // calling inserter is equivalent to back_inserted for std::vector.
          std::sample(in.begin(), in.end(), std::inserter(res, res.end()), n,
                      std::mt19937{std::random_device{}()});
        }
    }
  return res;
}

//! An example of use of random shuffling
//!
//! It returns a container with the elements of the input container
//! shuffler randomly.
//!
//! @note  The container must be a sequential container whose iterator
//! is a RandomIterator. It means that it can be a std::vector, a std::array
//! or (probably) even a C-style array.
//!
//! @tparam Cont  Any container
//! @param in The input container. Must have swappable elements. If it can be
//! moved it will.
//! @return a container of the same size, with shuffled elements
template <class Vector>
Vector
shuffleContainer(Vector &&in)
{
  std::random_device         rd;      // random seed (to add entropy)
  std::default_random_engine g{rd()}; // random number generator
  Vector                     res{std::forward<Vector>(in)}; // move if movable
  std::shuffle(res.begin(), res.end(), g);                  // shuffle it!
  return res;
}

} // end namespace apsc

#endif /* EXAMPLES_SRC_RANDOMDISTIBUTIONS_SAMPLING_HPP_ */
