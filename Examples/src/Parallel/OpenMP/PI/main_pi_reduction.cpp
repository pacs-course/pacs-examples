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
 * Same as main_pi.cpp, but using parallel reduction.
 */
void
printHelp()
{
  std::cout << "To run the program\n";
  std::cout << "./main_pi_reduction -t num_threads -e number_of_elemts\n";
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
    double x;
    double sum = 0.0;

  /**
   * We declare sum as a shared variable that has to be reduced
   * over the different threads, i.e. the value computed by each
   * thread is summed by the end of the parallel block.
   */
    Timings::Chrono clock;
    clock.start();
#pragma omp parallel for schedule(static), private(x), num_threads(num_threads),reduction(+ : sum)
  for(unsigned int i = 0; i < n; ++i)
    {
      x = h * (i + 0.5);
      sum += 4.0 / (1.0 + x * x);
    }

  // Now the main thread has the correct value of sum.
  const double pi = h * sum;
  auto spentTime=clock.wallTimeNow();

  std::cout << std::setprecision(16) << "pi = " << pi
            << ", error = " << std::abs(pi - pi_exact) << std::endl;
  std::cout<<"Time spent="<<spentTime<<"microsec.\n";

  return 0;
}
