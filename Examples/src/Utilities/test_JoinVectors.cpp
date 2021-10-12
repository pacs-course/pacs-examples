/*
 * test_joinVectors.cpp
 *
 *  Created on: Jan 21, 2021
 *      Author: forma
 */
#include "JoinVectors.hpp"
#include <algorithm>
#include <complex>
#include <iostream>
#include <numeric>
int
main()
{
  std::vector<int>                  vi;
  std::vector<double>               vd;
  std::vector<std::complex<double>> vc;
  auto                              joined = apsc::join_vectors(vi, vd, vc);
  int                               N = 10;
  vi.resize(N, 9);
  vd.resize(N, 3.4);
  vc.resize(N, {4.0, 3.0});
  vi.back() = 40;
  vd.back() = -10.;

  // Loop over the joined vectors
  std::cout << "*********** Joined vector content:\n";
  for(std::size_t i = 0; i < joined.size(); ++i)
    {
      auto [k, x, c] = joined[i];
      std::cout << k << " " << x << " " << c << std::endl;
    }
  std::cout << "*********** you can also loop via a range based for:\n";
  for(auto [k, x, c] : joined)
    std::cout << k << " " << x << " " << c << std::endl;
  std::cout << "*********** or a loop with iterators:\n";
  for(auto i = joined.begin(); i != joined.end(); ++i)
    {
      auto const [k, x, c] = *i;
      std::cout << k << " " << x << " " << c << std::endl;
    }
  // Use of the access through pointer operator (here the "pointer" is the
  // iterator).
  auto it = joined.begin();
  std::cout << "\n The size of the stored joned vectors is: " << it->size()
            << std::endl;
  //
  joined[2] = std::make_tuple(3, -67.0, std::complex<double>{99.0, -3.0});
  std::cout << "*********** After changing the 3rd element:\n";
  for(auto [k, x, c] : joined)
    std::cout << k << " " << x << " " << c << std::endl;

  // Some test of the other features
  auto start = joined.begin();
  *start = joined[2];
  std::cout << "*********** After changing the 1st element:\n";
  for(auto [k, x, c] : joined)
    std::cout << k << " " << x << " " << c << std::endl;
  // Use with std modifying algorithms
  std::for_each(joined.begin(), joined.end(), [](auto const &x) {
    auto [k, y, c] = x;
    y = 10.0 * y;
    return std::tie(k, y, c);
  });
  std::cout << "*********** After multiplying by 10:\n";
  for(auto [k, x, c] : joined)
    std::cout << k << " " << x << " " << c << std::endl;

  // to test the const version
  auto const joinedc = joined;
  std::cout << "*********** looping the const version" << std::endl;
  for(auto [k, x, c] : joinedc)
    std::cout << k << " " << x << " " << c << std::endl;
  // a non-modifying algorithm
  double result = std::accumulate(joined.begin(), joined.end(), 0.0,
                                  [](double const &a, auto const &b) {
                                    double y = std::get<1>(b);
                                    return a + y;
                                  });
  std::cout
    << " The sum af all elements of the second vector in the joined vector is"
    << result << std::endl;
}
