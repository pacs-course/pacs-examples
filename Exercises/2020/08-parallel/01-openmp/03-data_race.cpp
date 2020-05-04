#include <omp.h>

#include <iostream>

int
main(int argc, char **argv)
{
  constexpr size_t N = 5;

  int a[N];
  int b[N];

  // Serial version.
  for (size_t i = 0; i < N; ++i)
    {
      a[i] = i;
      b[i] = 2 * 1;
    }

  std::cout << "Serial:" << std::endl;
  int serial_checksum = 0;
  for (size_t i = 0; i < N - 1; ++i)
    {
      a[i] = a[i + 1] + b[i];

      std::cout << "i = " << i << ", a[" << i << "] = a[" << i + 1
                << "] + b[" << i << "] = " << a[i + 1] << " + "
                << b[i] << " = " << a[i] << std::endl;

      serial_checksum += a[i];
    }

  // Parallel version.
  for (size_t i = 0; i < N; ++i)
    {
      a[i] = i;
      b[i] = 2 * 1;
    }

  int n_threads;
#pragma omp parallel shared(a, b, n_threads)
  {
#pragma omp master
    {
      n_threads = omp_get_num_threads();
      std::cout << std::endl
                << "Parallel (number of threads: " << n_threads
                << "):" << std::endl;
    }
#pragma omp barrier

#pragma omp for
    for (size_t i = 0; i < N - 1; ++i)
      {
        /**
         * Possible data race: if another thread overwrites a[i + 1]
         * before the current thread computes a[i],
         * the result will be wrong.
         */
        a[i] = a[i + 1] + b[i];

#pragma omp critical
        {
          std::cout << "i = " << i << ", a[" << i << "] = a[" << i + 1
                    << "] + b[" << i << "] = " << a[i + 1] << " + "
                    << b[i] << " = " << a[i] << std::endl;
        }
      }
  }

  int parallel_checksum = 0;
#pragma omp parallel for reduction(+ : parallel_checksum)
  for (size_t i = 0; i < N - 1; ++i)
    {
      parallel_checksum += a[i];
    }

  std::cout << std::endl
            << "Serial   result: " << serial_checksum << std::endl
            << "Parallel result: " << parallel_checksum << std::endl;
}
