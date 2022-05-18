#include <omp.h>

#include <iostream>

int
main(int argc, char **argv)
{
  int thread_id;
  int n_threads;

/**
 * Fork a team of threads with each thread
 * having a private thread_id variable.
 */
#pragma omp parallel private(thread_id) shared(n_threads)
  {
    thread_id = omp_get_thread_num();

/**
 * The following block is "critical", i.e. we only let one thread
 * execute this block at a time.
 *
 * Other types of blocks include single (only one thread executes
 * this block), master (only the master thread executes this
 * block), atomic (protect a variable by changing it in one step).
 */
#pragma omp critical
    std::cout << "Hello World from thread " << thread_id << "!" << std::endl;


// Let's wait until all threads reach the following line.
#pragma omp barrier

/**
 * Only one single thread (the first one available) prints this.
 * Additionally, "#pragma omp master" would ensure that the actual thread
 * executing this block is the one with thread_id equal to 0 (it would be
 * equivalent to "if (thread_id == 0)".
 */
#pragma omp single
    {
      n_threads = omp_get_num_threads();
      std::cout << "Number of threads = " << n_threads << std::endl;
    }
  }

  // All threads join master thread and terminate.

  return 0;
}
