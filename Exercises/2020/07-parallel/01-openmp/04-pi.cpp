#include <omp.h>

#include <ctime>
#include <iomanip>
#include <iostream>

static clock_t c_start, c_diff;
static double  c_sec;
#define tic() c_start = clock();
#define toc(x)                                      \
  c_diff = clock() - c_start;                       \
  c_sec  = (double)c_diff / (double)CLOCKS_PER_SEC; \
  std::cout << x << c_sec << " [s]" << std::endl;

/**
 * In this example we perform the integral of 4 / (1 + x^2) between 0
 * and 1; this should equal pi. We sum over the integral range in a
 * for loop, which we parallelize using OpenMP.
 */
int
main(int argc, char **argv)
{
  double pi  = 0.0;
  double sum = 0.0;
  double x;

  const unsigned int N = 1e8;
  const double       w = 1.0 / N;

  tic();

/**
 * We declare x as private, i.e. each thread keeps its own copy.
 * Private variables are NOT initialized, we can declare them
 * outside the parallel block's scope, but they must be initialized
 * in each thread.
 *
 * We declare sum as firstprivate, i.e. it's private but intialized
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
#pragma omp for schedule(auto)
    for (unsigned int i = 0; i < N; ++i)
      {
        x = w * (i + 0.5);
        sum += 4.0 / (1.0 + x * x);
      }

#pragma omp critical
    {
      pi += w * sum;
    }
  }

  std::cout << std::setprecision(16) << "pi = " << pi << "\n";

  toc("Time elapsed: ");

  return 0;
}
