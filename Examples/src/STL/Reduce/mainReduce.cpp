/*
 * mainReduce.cpp
 *
 *  Created on: Apr 5, 2020
 *      Author: forma
 */
#include<numeric>
#include<iostream>
#include<execution>
#include <random>
#include<vector>
#include "chrono.hpp"
//Try to change the execution policy to verify the effects of parallelization
// to have the scalar version -DSCALAR
#ifdef SCALAR
#define EXECUTION std::execution::seq
#warning "Scalar execution"
#else
#define EXECUTION std::execution::par
#endif
//! Utility function that creates a vector with random values
std::vector<double> randomVector(std::size_t n)
{
  Timings::Chrono timer;
  timer.start();
  std::random_device rd;
  std::default_random_engine engine{rd()};
  std::uniform_real_distribution uniform_dist{-10.0, 10.0};
  std::vector<double> res;
  res.reserve(n);
  // I cannot use for_each in parallel fashion since the engine is not stateless
  for (std::size_t i=0u; i<n; ++i) res.emplace_back(uniform_dist(engine));
  timer.stop();
  std::cout<<" Time taken to create vector:"<<timer.wallTime()<<" microsec"<<std::endl;
  return res;
}
int main()
{
  unsigned int n = 10000000;
  std::vector<double> bigVector{randomVector(n)};
  // Example of simple use of a parallel reduce: add all values
  Timings::Chrono timer;
  timer.start();
  std::cout<<"Sum= "<<
      std::reduce(EXECUTION, // execution policy
		  std::begin(bigVector), // start
		  std::end(bigVector),  //end
		  0.0,                  // initial value
		  std::plus{});//binary operator (sum, not needed since it is the default
   std::cout<<std::endl;
   timer.stop();
   std::cout<<" Time taken:"<<timer.wallTime()<<" microsec"<<std::endl;

  // A more interesting algorithm, transform_reduce. In this form it applies an unitary operation to all values
  // in the range and then applies a reduction using a binary operation. In this example we compute the 1-norm

  // A lambda for the unary operator

  auto absolute = [](double const & x){return std::abs(x);};
  timer.start();
  std::cout<<"1-Norm= "<<
       std::transform_reduce(EXECUTION, // execution policy
 		  std::begin(bigVector), // start
 		  std::end(bigVector),  //end
 		  0.0,                  // initial value
 		  std::plus{},//binary operator
                  absolute); // the lambda as unary operator
  std::cout<<std::endl;
  timer.stop();
  std::cout<<" Time taken:"<<timer.wallTime()<<" microsec"<<std::endl;
  // Another type of transform_reduce: it does the inner product
  auto secondVector{randomVector(n)};
  timer.start();
  std::cout<<"Dot product= "<<
         std::transform_reduce(EXECUTION, // execution policy
   		  std::begin(bigVector), // start first
   		  std::end(bigVector),  //end firse
		  std::begin(secondVector), // begin second
   		  0.0); // Initial Value
  std::cout<<std::endl;
  timer.stop();
  std::cout<<" Time taken:"<<timer.wallTime()<<" microsec"<<std::endl;

  // This is the most complex form.
  // It applies a binary operation to all couple of elements of the two ranges
  // then a reduction applying a second binary operation consecutively on the result.
  // For instance here we compute the 1-norm of the difference ||A-B||_1
  auto absDiff = [](const double & x, const double & y){return std::abs(x-y);};

    timer.start();
  std::cout<<"|a-b|_1= "<<
          std::transform_reduce(EXECUTION, // execution policy
    		  std::begin(bigVector), // start first
    		  std::end(bigVector),  //end firse
 		  std::begin(secondVector), // begin second
    		  0.0, // Initial Value
		  std::plus{}, // binary operation performd for reduction
		  absDiff // binary operation performed on each couple of elements
		  );
  timer.stop();

  std::cout<<std::endl;
  std::cout<<" Time taken:"<<timer.wallTime()<<" microsec"<<std::endl;

}

