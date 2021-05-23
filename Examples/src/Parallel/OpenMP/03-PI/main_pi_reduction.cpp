#include <omp.h>

#include <cmath>
#include <iomanip>
#include <iostream>

/**
 * Same as 04-pi.cpp, but using parallel reduction.
 */
int
main(int argc, char **argv)
{
  const unsigned int n = 1e9;
  const double       h = 1.0 / n;

  double x;
  double sum = 0.0;

  /**
   * We declare sum as a shared variable that has to be reduced
   * over the different threads, i.e. the value computed by each
   * thread is summed by the end of the parallel block.
   */
#pragma omp parallel for schedule(static), private(x), reduction(+ : sum)
  for (unsigned int i = 0; i < n; ++i)
    {
      x = h * (i + 0.5);
      sum += 4.0 / (1.0 + x * x);
    }

  // Now the main thread has the correct value of sum.
  const double pi = h * sum;

  std::cout << std::setprecision(16) << "pi = " << pi
            << ", error = " << std::abs(pi - M_PI) << std::endl;

  return 0;
}
