/*
 * fibonacci.cpp
 *
 *  Created on: May 21, 2019
 *      Author: forma
 */
#include "fibonacci.hpp"
unsigned long int
FibonacciNR (unsigned int const N)
{
  unsigned long int first = 1ul;
  unsigned long int second = first;
  if (N > 3)
    {
      for (unsigned int i = 3; i <= N; ++i)
        {
          auto save = second;
          second += first;
          first = save;
        }
    }
  return second;
}

