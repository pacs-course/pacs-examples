#include <omp.h>

#include <cmath>
#include <iomanip>
#include <iostream>

/**
 * This exercise presents a simple program to determine the value of
 * pi. The algorithm suggested here is chosen for its simplicity. The
 * method evaluates the integral of 4 / (1 + x^2) between 0 and 1
 * using the composite midpoint rule. We sum over the integral range
 * in a for loop, parallelized using OpenMP.
 */
int
main(int argc, char **argv)
{
  const unsigned int n = 1e8;
  const double       h = 1.0 / n;

  double x;
  double sum = 0.0;
  double pi  = 0.0;

/**
 * We declare x as private, i.e. each thread keeps its own copy.
 * Private variables are NOT initialized, we can declare them
 * outside the parallel block's scope, but they must be initialized
 * in each thread.
 *
 * We declare sum as firstprivate, i.e. it's private but initialized
 * with the value defined in the master thread; 0.0 in this case.
 *
 * We declare pi as shared, any thread can access it. Shared is the
 * default, however being explicit is not a bad thing. You should be
 * careful that multiple threads are not writing to shared variables
 * at the same time (race conditions).
 */
#pragma omp parallel private(x), firstprivate(sum), shared(pi)
  {
/**
 * See http://jakascorner.com/blog/2016/06/omp-for-scheduling.html
 * for an explanation about scheduling types.
 */
#pragma omp for schedule(static)
    for (unsigned int i = 1; i <= n; ++i)
      {
        x = h * (i - 0.5);
        sum += 4.0 / (1.0 + x * x);
      }

// Critical block, to prevent data race!
#pragma omp critical
    {
      pi += h * sum;
    }
  }

  std::cout << std::setprecision(16) << "pi = " << pi << ", error = "
            << std::fabs(pi - 3.141592653589793238462643)
            << std::endl;

  return 0;
}
