#include <omp.h>

#include <iostream>

int
main(int argc, char **argv)
{
  int n_threads;
  int thread_id;

  // Fork a team of threads with each thread
  // having a private thread_id variable.
#pragma omp parallel private(thread_id)
  {
    thread_id = omp_get_thread_num();
#pragma omp critical
    std::cout << "Hello World from thread " << thread_id << "!"
              << std::endl;

#pragma omp barrier

    // Only master thread does this.
    if (thread_id == 0)
      {
        n_threads = omp_get_num_threads();
        std::cout << "Number of threads = " << n_threads << std::endl;
      }
  } // All threads join master thread and terminate.

  return 0;
}
