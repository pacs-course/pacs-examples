/*
 * Utilities.hpp
 *
 *  Created on: Aug 22, 2022
 *      Author: forma
 */

#ifndef AMSC_EXAMPLES_EXAMPLES_SRC_PARALLEL_OPENMP_SORT_UTILITIES_HPP_
#define AMSC_EXAMPLES_EXAMPLES_SRC_PARALLEL_OPENMP_SORT_UTILITIES_HPP_
#include <algorithm>
#include <array>
#include <random>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
#ifdef CPP_PARALLEL
#include <execution> // for C++ native multithreading
#endif
#include "omp.h"
namespace vectorUtil_OMP
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
 * @tparam T the type stored in the vector
 * @param v In input the vector, in output the sorted vector
 */
template <class T>
void
sort_vec(std::vector<T> &v)
{
  std::sort(std::begin(v), std::end(v));
}

#ifdef CPP_PARALLEL
/*!
 *  The parallel version
 * @tparam T
 * @param a
 */
template <class T>
void
sort_vec_par(std::vector<T> &v)
{
  std::sort(std::execution::par, std::begin(v), std::end(v));
}
#endif

/*!
 * Odd even transposition sort adapted from the code in An Introduction to parallel Programming (2nd edition)
 * by P.S. Packeco and M. Melensk
 * @tparam T The type of the vector to sort
 * @param a The vector
 */
template <class T>
void Odd_even(std::vector<T> & a)
{
  auto n = a.size();
  if(n==0u) return;
#pragma omp parallel default(none) shared(a, n, std::cout)
  {
#pragma omp master
      std::cout<<"Using "<<omp_get_num_threads()<<"threads\n";
      for (auto phase = 0u; phase < n; phase++) {
          if (phase % 2 == 0)
#pragma omp for
            for (auto i = 1u; i < n; i += 2u) {
                if (a[i-1] > a[i]) {
                    auto tmp =a[i];
                    a[i]=a[i+1];
                    a[i+1]=tmp;
                    //std::swap(a[i],a[i-i]);
                }
            }
          else
#pragma omp for
            for (auto i = 1u; i < n-1u; i += 2u) {
                if (a[i] > a[i+1]) {
                    auto tmp =a[i];
                    a[i]=a[i-1];
                    a[i-1]=tmp;
                   //std::swap(a[i],a[i+i]);
                }
            }
      }
  }
}

// for this implementation of Qsort we need to avoid an excessive number of tasks activated
// If the size of the partion is less than TASK_LIMIT we do a standard sort.
/*
 * This code is a modification of the code available in https://github.com/Michael-Beukman/OpenMPQuicksort
 * by Michael Beukman. The main modification is the use of std:vector instead of pointers, to be more
 * C++ style.
 * In the original code you have  version that uses sections instead of tasks, but it is less efficient.
 *
 *
 */
#define TASK_LIMIT 1000

/*!
 * Helper utility that performs the partition of the data, and identifies the current pivot
 *
 * @tparam T The type of the vector
 * @param p The current pivot index
 * @param r The right end of the current partition
 * @param data The vector
 * @return The new pivot index.
 */
template <class T>
int partition(int p, int r, std::vector<double>& data){
    T pivot = data[p];
    int left = p;
    int right = r;
    while (left < right){
        // increase until you get to a point where the element is greater that the pivot
        while (left < r && data[left] <= pivot) ++left;
        // decrease until you get to a point where the element is less or equal to the pivot
        while (right >= 0 && data[right] > pivot) --right;
        // swap if within bounds
        if (left < right && left <r && right >= 0){
            std::swap(data[left], data[right]);
        }
    }

    // swap at the end
    if (left < right && left <r && right >= 0){
        std::swap(data[left], data[right]);
    }
    data[p] = data[right];
    data[right] = pivot;
    return right;
}

/*!
 * Sequential implementation of quicksort
 * @tparam T The type of the vector's elements.
 * @param p The left index
 * @param r The right index
 * @param data the vector
 */
template <class T>
void seq_qsort(int p, int r, std::vector<T> & data) {
    if (p < r) {
        int q = partition<T>(p, r, data);
        seq_qsort(p, q - 1, data);
        seq_qsort(q + 1, r, data);
    }
}
/*!
 * The openMP task that implements quicksort
 *
 * If the partition is small we do scalar sort
 * @tparam T
 * @param p
 * @param r
 * @param data
 * @param low_limit The limit below which we go scalar
 */
template <class T>
void q_sort_tasks(int p, int r, std::vector<T> & data, int low_limit) {
  if (p < r) {
      if (r - p < low_limit)
        {
          // small list => sequential.
          seq_qsort<T>(p, r, data);
        }else{

            int q = partition<T>(p, r, data);
            // create two tasks
#pragma omp task shared(data)
            q_sort_tasks<T>(p, q - 1, data, low_limit);
#pragma omp task shared(data)
            q_sort_tasks<T>(q + 1, r, data, low_limit);
        }
  }
}
/*!
 * The main driver
 * @tparam T
 * @param p
 * @param r
 * @param data
 */
template <class T>
void par_q_sort_tasks(int p, int r, std::vector<T>& data){
#pragma omp parallel
        {
            #pragma omp single
            q_sort_tasks<T>(p, r, data, TASK_LIMIT - 1);
            #pragma omp taskwait
        }
}
template <class T>
void parallelQuickSort(std::vector<T>& data)
{
  par_q_sort_tasks(0,data.size()-1u,data);
}


}




#endif /* AMSC_EXAMPLES_EXAMPLES_SRC_PARALLEL_OPENMP_SORT_UTILITIES_HPP_ */
