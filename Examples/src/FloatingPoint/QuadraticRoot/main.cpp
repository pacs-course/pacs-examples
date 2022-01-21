/*
 * main.cpp
 *
 *  Created on: 22 Aug 2011
 *      Author: formaggia
 */
#include "QuadraticRoot.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
// Computing residuals in high precision
void
printResidual(result_type y1, result_type y2, Real aa, Real bb, Real cc)
{
  auto a = static_cast<long double>(aa);
  auto b = static_cast<long double>(bb);
  auto c = static_cast<long double>(cc);
  auto x1 = std::complex<long double>(static_cast<long double>(y1.real()),
                                      static_cast<long double>(y1.imag()));
  auto x2 = std::complex<long double>(static_cast<long double>(y2.real()),
                                      static_cast<long double>(y2.imag()));
  long double factor1 = std::abs(x1) == static_cast<long double>(0.0)
                          ? static_cast<long double>(1.0)
                          : std::abs(x1);
  long double factor2 = std::abs(x2) == static_cast<long double>(0.0)
                          ? static_cast<long double>(1.0)
                          : std::abs(x2);
  std::cout << "Relative Residuals: " << std::endl
            << "r1= " << std::abs(a * x1 * x1 + b * x1 + c) / factor1
            << ", r2= " << std::abs(a * x2 * x2 + b * x2 + c) / factor2
            << std::endl
            << std::endl;
}

int
main()
{
  using namespace std;
  cout << "I will compute the root of ax^2+b^x+c" << endl;
  cout << "This version uses floating point type of size " << sizeof(Real)
       << " bytes" << endl;
  Real                                a(0);
  Real                                b(1);
  Real                                c(0);
  std::pair<result_type, result_type> result;
  cout.setf(ios::scientific); // use scientific float format
TryAgain:
  while(true)
    {
      cout << "Give me a, b and c (all zeros to end)" << endl;
      // Beware: this is not safe. If you give in input data
      // not convertible to Reals you get unpredictable result
      // A better implementation uses getline and istringstreams
      // Or GetPot!
      cin >> a >> b >> c;
      if(cin.fail())
        {
          cerr << "Wrong input: try again" << endl;
          // If the input stream is screwed up
          // it is difficult to bring it back to a good state.
          // A possibility is to read everithing up to the end of the
          // budder and try again. First clear the state flags!
          cin.clear();
          while(!cin.eof())
            cin.ignore();
          // Normally you should try to avoid goto's but this
          // is one of the few cases where a goto makes the code
          // easier to read.
          goto TryAgain;
        }
      // C++11 feature: max may take a parameter list!
      if(std::max({a, b, c}) != 0.0)
        {
          try
            {
              // Compute by less stable method
              result = quadraticRoot_simple(a, b, c);
              cout << " With naive method:" << endl;
              cout << "x1=" << result.first << " x2=" << result.second << endl;
              printResidual(result.first, result.second, a, b, c);
              // Compute with more stable method
              cout << " With more stable method:" << endl;
              result = quadraticRoot(a, b, c);
              cout << "x1=" << result.first << " x2=" << result.second << endl
                   << endl;
              // Residual is always computed in extended precision
              printResidual(result.first, result.second, a, b, c);
            }
          catch(std::runtime_error &error)
            {
              cerr << error.what() << endl;
              goto TryAgain;
            }
        }
      else
        {
          exit(0);
        }
    }
}
