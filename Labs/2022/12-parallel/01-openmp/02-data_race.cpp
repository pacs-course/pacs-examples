#include <omp.h>

#include <iostream>
#include <vector>

/**
 * This exercise presents a simple algorithm that cannot be parallelized.
 * Any attempt to parallelize it would result in a data race (or race
 * condition).
 */
int
main(int argc, char **argv)
{
  const size_t n = 10;

  std::vector<unsigned int> a(n);
  std::vector<unsigned int> b(n);

  // Serial version.
  for (size_t i = 0; i < n; ++i)
    {
      a[i] = i;
      b[i] = 2 * i;
    }

  std::cout << "Serial:" << std::endl;
  int serial_checksum = 0;
  for (size_t i = 0; i < n - 1; ++i)
    {
      a[i] = a[i + 1] + b[i];

      std::cout << "i = " << i << ", a[" << i << "] = a[" << i + 1 << "] + b["
                << i << "] = " << a[i + 1] << " + " << b[i] << " = " << a[i]
                << std::endl;

      serial_checksum += a[i];
    }

  // Parallel version.
  for (size_t i = 0; i < n; ++i)
    {
      a[i] = i;
      b[i] = 2 * i;
    }

  int thread_id;
  int n_threads;
#pragma omp parallel shared(a, b, n_threads) private(thread_id)
  {
    thread_id = omp_get_thread_num();

#pragma omp master
    {
      n_threads = omp_get_num_threads();
      std::cout << std::endl
                << "Parallel (number of threads: " << n_threads
                << "):" << std::endl;
    }
#pragma omp barrier

#pragma omp for
    for (size_t i = 0; i < n - 1; ++i)
      {
        /**
         * Possible data race: if another thread overwrites a[i + 1]
         * (or a[i - 1]) before the current thread computes a[i],
         * the result will likely be wrong.
         */
        a[i] = a[i + 1] + b[i];

#pragma omp critical
        {
          std::cout << "thread " << thread_id << ", i = " << i << ", a[" << i
                    << "] = a[" << i + 1 << "] + b[" << i << "] = " << a[i + 1]
                    << " + " << b[i] << " = " << a[i] << std::endl;
        }
      }
  }

  int parallel_checksum = 0;
#pragma omp parallel for reduction(+ : parallel_checksum)
  for (size_t i = 0; i < n - 1; ++i)
    {
      parallel_checksum += a[i];
    }

  std::cout << std::endl
            << "Serial   checksum: " << serial_checksum << std::endl
            << "Parallel checksum: " << parallel_checksum << std::endl;
}
