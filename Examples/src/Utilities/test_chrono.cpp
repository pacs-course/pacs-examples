#include "chrono.hpp"
#include <iostream>
#include <vector>
int
main()
{
  Timings::Chrono     clock;
  std::vector<double> bigVector;
  clock.start();
  for(std::size_t i = 0; i < 100000; ++i)
    bigVector.push_back(3.0);
  double sum(0);
  for(auto i : bigVector)
    sum += i;
  std::cout << "Elapsed time " << clock.wallTimeNow() << " microseconds"
            << std::endl;
  double prod(1);
  for(auto i : bigVector)
    prod *= i;
  clock.stop();
  std::cout << clock;
}
