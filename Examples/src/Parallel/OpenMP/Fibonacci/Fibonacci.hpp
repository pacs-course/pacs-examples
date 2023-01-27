/*
 * Fibonacci.hpp
 *
 *  Created on: Aug 26, 2022
 *      Author: forma
 */

#ifndef AMSC_EXAMPLES_EXAMPLES_SRC_PARALLEL_OPENMP_FIBONACCI_FIBONACCI_HPP_
#define AMSC_EXAMPLES_EXAMPLES_SRC_PARALLEL_OPENMP_FIBONACCI_FIBONACCI_HPP_
#include <vector>

namespace Fibonacci
{
// Teo namespace variables. Not very nice but useful
// Alternative: a struct cwith the two data
//! The number of threads
inline int num_threads{1};
//! The limit below which we use a serial procedure
inline unsigned int par_limit{1u};
/*!
 * A class that implements the computation of Fibonacci sequence
 * with a recursive algorithm made parallel with the use of OpenMP tasks.
 */
class Fibonacci
{
public:
/*!
 * The main driver for the computation
 * @param n The maximal index of the sequence
 * @return The vector with the sequence
 */
  auto const &
  compute(long unsigned int n)
  {
    //! resize the vector to store the sequence
    fibs.resize(n+1ul,0ul);
#pragma omp parallel num_threads(num_threads)
#pragma omp single
    fib(n);
return fibs;
  }


private:
/*!
 * The implementation of the recursive algorithm for the computation of Fibonacci sequance.
 * It updates the member variable fibs with F(n) and returns F(n)
 * @param n The index of the Fibonacci number to be computed
 * @return The Fibonacci number F(n)
 */
  long unsigned int fib(long unsigned int n)
  {
    long unsigned int i{0ul};
    long unsigned int j{0ul};
    if(n<=1ul)
      {
        fibs[n]=n;
        return n;
      }

#pragma omp task shared(i) if (n>par_limit)
    i = fib(n-1ul);
#pragma omp task shared(j) if (n>par_limit)
    j = fib(n-2ul);
#pragma omp taskwait
    fibs[n]= i+j;
    return fibs[n];
  }
  //! The vector storing the sequence
  std::vector<long unsigned int> fibs;
};

/*!
 * Serial version, implementing the same recursive algorithm
 */
class Fibonacci_s
{
public:
 auto const &
  compute(long unsigned int n)
  {
    fibs.resize(n+1ul,0ul);
    fib(n);
return fibs;
  }

private:
  long unsigned int fib(long unsigned int n)
  {
    long unsigned int i{0ul};
    long unsigned int j{0ul};
    if(n<=1ul)
      {
        fibs[n]=n;
        return n;
      }

    i = fib(n-1ul);
    j = fib(n-2ul);
    fibs[n]= i+j;
    return fibs[n];
  }
  std::vector<long unsigned int> fibs;
};
/*!
 * Serial version of Fibonacci, implementing a non recursive algorithm
 */
class Fibonacci_nonrec
{
public:
 auto const &
  compute(long unsigned int n)
  {
    fibs.resize(n+1ul,0ul);
    fib(n);
return fibs;
  }

private:
  long unsigned int fib(long unsigned int n)
  {
    fibs[0]=0ul;
    if (n==0ul) return 0ul;
    fibs[1]=1ul;
    if (n==1ul) return 1ul;
    for (std::size_t i=2;i<=n;++i)
      fibs[i]=fibs[i-1ul]+fibs[i-2ul];
    return fibs[n];
  }
  std::vector<long unsigned int> fibs;
};

}


#endif /* AMSC_EXAMPLES_EXAMPLES_SRC_PARALLEL_OPENMP_FIBONACCI_FIBONACCI_HPP_ */
