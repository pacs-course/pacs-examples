#include <omp.h>

#include <cmath>
#include <iomanip>
#include <iostream>
#include "GetPot"
#include "chrono.hpp"
#if __cplusplus >= 202002L
#include <numbers>
#endif
/**
 * This exercise presents a simple program to determine the value of
 * pi. The algorithm suggested here is chosen for its simplicity. The
 * method evaluates the integral of 4 / (1 + x^2) over the interval (0, 1)
 * using the composite midpoint rule. We sum over the integral range
 * in a for loop, parallelized using OpenMP.
 */
void
printHelp()
{
  std::cout << "To run the program\n";
  std::cout << "./main_pi -t num_threads -e number_of_elemts\n";
  std::cout
    << "num_threads: number of threads requested (def. 2),\n"
    << "number_of_elemts: The number of elements for the integration, use a "
       "big number. default: 1e9)\n"
    << std::endl;
}
int
main(int argc, char **argv)
{
// Since C++20 we have mathematical constants defined as C++ constexpr
#if __cplusplus >= 202002L
  constexpr auto pi_exact = std::numbers::pi_v<long double>;
#else // I rely on old C stuff
  constexpr auto pi_exact = M_PI;
#endif
  double x;
  double sum = 0.0;
  double pi = 0.0;
  GetPot gp(argc, argv);
  if(gp.search(2, "-h", "--help"))
    {
      printHelp();
      return 0;
    }
  unsigned int n = gp.follow(1000000000u, "-e");
  unsigned int num_threads = gp.follow(2u, "-t");
  std::cout<<"Using "<<num_threads<<" threads and "<< n<<" elements\n";
  const double       h = 1.0 / n;

  std::cout<<"Number of elements for the computation of Pi="<<n<<std::endl;
  std::cout<<"Number of threads requested="<<num_threads<<std::endl;
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
 * default variable not local to the paralell block,
 * however being explicit is not a bad thing. You should be
 * careful that multiple threads are not writing to shared variables
 * at the same time (race conditions, as in exercise 03).
 *
 * Specifying default(none) is also a good practice, so the compiler
 * forces us to specify whether each of the variables used herein
 * are private or shared.
 */
Timings::Chrono clock;
clock.start();
#pragma omp parallel private(x), firstprivate(sum), shared(pi,h) num_threads(num_threads)
  {
/**
 * See http://jakascorner.com/blog/2016/06/omp-for-scheduling.html
 * for a description of all possible scheduling types.
 */
#pragma omp for schedule(static)
    for(unsigned int i = 0; i < n; ++i)
      {
        x = h * (i + 0.5);
        sum += 4.0 / (1.0 + x * x);
      }

// Critical block, to prevent data race!
// Atomic is just a shorthand for a critical variable read/write operation.
#pragma omp atomic update
    pi += h * sum;
  }
auto spentTime=clock.wallTimeNow();
  std::cout << std::setprecision(16) << "pi = " << pi
            << ", error = " << std::abs(pi - pi_exact) << std::endl;
  std::cout<<"Time spent="<<spentTime<<"microsec.\n";
  return 0;
}
