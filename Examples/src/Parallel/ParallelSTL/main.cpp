/*
 * main.cpp
 *
 *  Created on: Feb 6, 2021
 *      Author: forma
 */
#include "chrono.hpp"
#include <algorithm>
#include <execution>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
inline void fill_random(std::vector<double> & v)
{
  std::random_device rd{};
  std::mt19937 gen{rd()};
  std::uniform_real_distribution<double> d{-1.0,1.0};
  for (auto & x:v)
    x=d(gen);
}

int
main()
{
  // Some paralllel algorithms of the standard library
  // They operate on ranges
  std::vector<double> a;
  a.resize(10000000); // A big vector filled with random values
  fill_random(a);
  Timings::Chrono watch;
  // Sum all elements using reduce
  {
    watch.start();
    double res = std::reduce(std::execution::seq, a.begin(), a.end(), 0.);
    watch.stop();
    std::cout << "Summing " << a.size()
              << " element with sequential execution\n";
    std::cout << "Result=" << res << std::endl;
    std::cout << watch << std::endl;
  }
  {
    watch.start();
    double res = std::reduce(std::execution::par, a.begin(), a.end(), 0.);
    watch.stop();
    std::cout << "Summing " << a.size() << " element with parallel execution\n";
    std::cout << "Result=" << res << std::endl;
    std::cout << watch << std::endl;
  }
  // product all elements using reduce
  a.resize(a.size(), 1.1);
  {
    watch.start();
    double res =
      std::reduce(std::execution::seq, a.begin(), a.end(), 1.,
                  [](double const &x, double const &y) { return x * y; });
    watch.stop();
    std::cout << "Multiplying " << a.size()
              << " element with sequential execution\n";
    std::cout << "Result=" << res << std::endl;
    std::cout << watch << std::endl;
  }
  {
    watch.start();
    double res =
      std::reduce(std::execution::par, a.begin(), a.end(), 1.,
                  [](double const &x, double const &y) { return x * y; });
    watch.stop();
    std::cout << "Multiplying " << a.size()
              << " element with parallel execution\n";
    std::cout << "Result=" << res << std::endl;
    std::cout << watch << std::endl;
  }
  std::vector<double> b(a.size(),
                        2.5); // another big vector filled with a value (2.5)
                              // parallel inner product
  {
    watch.start();
    double res = std::transform_reduce(std::execution::seq, a.begin(), a.end(),
                                       b.begin(), 0.);
    watch.stop();
    std::cout << "Dot product of two vectors of " << a.size()
              << " element with sequential execution\n";
    std::cout << "Result=" << res << std::endl;
    std::cout << watch << std::endl;
  }
  {
    watch.start();
    double res = std::transform_reduce(std::execution::par, a.begin(), a.end(),
                                       b.begin(), 0.);
    watch.stop();
    std::cout << "Dot product of two vectors of " << a.size()
              << " element with parallel execution\n";
    std::cout << "Result=" << res << std::endl;
    std::cout << watch << std::endl;
  }
  // 1- norm of a-b
  {
    // std::plus<> is a functor that defines the addition equivalent to
    // [](double const & x, double const & y){return x+y;}
    watch.start();
    double res = std::transform_reduce(
      std::execution::seq, a.begin(), a.end(), b.begin(), 0., std::plus<>(),
      [](double const &x, double const &y) { return std::abs(x - y); });
    watch.stop();
    std::cout << "1-norm distance of two vectors of " << a.size()
              << " element with sequential execution\n";
    std::cout << "Result=" << res << std::endl;
    std::cout << watch << std::endl;
  }

  {
    // std::plus<> is a functor that defines the addition equivalent to
    // [](double const & x, double const & y){return x+y;}
    watch.start();
    double res = std::transform_reduce(
      std::execution::par, a.begin(), a.end(), b.begin(), 0., std::plus<>(),
      [](double const &x, double const &y) { return std::abs(x - y); });
    watch.stop();
    std::cout << "1-norm distance of two vectors of " << a.size()
              << " element with parallel execution\n";
    std::cout << "Result=" << res << std::endl;
    std::cout << watch << std::endl;
  }

  // add 10 to all elements of a
  {
    watch.start();
    std::for_each(std::execution::seq, a.begin(), a.end(),
                  [](double &x) { x += 10.; });
    watch.stop();
    std::cout << "adding a value to all element of a vectors of " << a.size()
              << " element with sequential execution\n";
    std::cout << watch << std::endl;
  }

  {
    watch.start();
    std::for_each(std::execution::par, a.begin(), a.end(),
                  [](double &x) { x += 10.; });
    watch.stop();
    std::cout << "adding a value to all element of a vectors of " << a.size()
              << " element with parallel execution\n";
    std::cout << watch << std::endl;
  }

  // create a vector contanint 1,2,3 etc
  std::iota(a.begin(), a.end(), 1.);
  // shuffle values
  std::random_device rd;
  std::mt19937       g(rd());
  std::shuffle(a.begin(), a.end(), g);
  b = a;
  // Now sort in descending order (thats why I use std::greater<>, the default
  // std::less gives ascenting order) This is just to show that sort may take a
  // ordering relation as last argument

  {
    watch.start();
    std::sort(std::execution::seq, a.begin(), a.end(), std::greater<>());
    watch.stop();
    std::cout << "sorting a vectors of " << a.size()
              << " element with sequential execution\n";
    std::cout << watch << std::endl;
  }
  {
    watch.start();
    std::sort(std::execution::par, b.begin(), b.end(), std::greater<>());
    watch.stop();
    std::cout << "sorting a vectors of " << b.size()
              << " element with parallel execution\n";
    std::cout << watch << std::endl;
  }

  //
}
