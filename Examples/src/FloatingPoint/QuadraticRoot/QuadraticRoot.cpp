/*
 * QuadraticRoot.cpp
 *
 *  Created on: 22 Aug 2011
 *      Author: formaggia
 */
#include "QuadraticRoot.hpp"
#include <cmath>
#include <stdexcept>
#include <utility>

std::pair<result_type, result_type>
quadraticRoot_simple(const Real &a, const Real &b, const Real &c)
{
  if(a == 0.)
    {
      if(b == 0.)
        throw std::runtime_error("Degenerate quadratic form");
      return std::make_pair(result_type{-c / b, 0.}, result_type{-c / b, 0.});
    }
  else
    {
      result_type discriminant{b * b - 4 * a * c};
      discriminant = std::sqrt(discriminant);
      result_type x1 = (-b + discriminant) /(2*a);
      result_type x2 = (-b - discriminant) /(2*a);
      // return std::make_pair(x1, x2); Before c++14
      return {x1,x2};
    }
}

std::pair<result_type, result_type>
quadraticRoot(const Real &a, const Real &b, const Real &c)
{
  result_type x1, x2;
  result_type discriminant = b * b - 4 * a * c;
  discriminant = std::sqrt(discriminant);
  if(a == 0)
    {
      if(b == 0)
        throw std::runtime_error("Degenerate quadratic form");
      return std::make_pair(result_type{-c / b, 0}, result_type{-c / b, 0});
    }
  else
    // I consider two cases to reduce errors
    {
      if(b > 0.)
        {
          x2 = -(b + discriminant) / (2*a);
          x1 = c / (a * x2);
        }
      else
        {
          x1 = (-b + discriminant) / (2*a);
          x2 = c / (a * x1);
        }
      //return std::make_pair(x1, x2);
      return {x1,x2};
    }
}
