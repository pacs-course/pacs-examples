/*
 * QuadraticRoot.cpp
 *
 *  Created on: 22 Aug 2011
 *      Author: formaggia
 */
#include <cmath>
#include <utility>
#include "QuadraticRoot.hpp"

std::pair<Real,Real>
quadraticRoot_simple(const Real & a, const Real & b, const Real & c)
throw(negativeDiscriminant)
{
  Real discriminant=b*b-4*a*c;
  if (discriminant <0.0) throw(negativeDiscriminant() );
  
  discriminant=std::sqrt(discriminant);
  if (a==0){
    return std::make_pair(-c/b,-c/b);
  }
  else
    {
      Real x1=(-b+discriminant)/(2*a);
      Real x2=(-b-discriminant)/(2*a);
      return std::make_pair(x1,x2);
    }
}

std::pair<Real,Real>
quadraticRoot(const Real & a, const Real & b, const Real & c)
  throw(negativeDiscriminant)
{
  Real x1,x2;
  Real discriminant=b*b-4*a*c;
  if (discriminant <0.0) throw(negativeDiscriminant() );
  discriminant=std::sqrt(discriminant);
  if (a==0){
    return std::make_pair(-c/b,-c/b);
  }
  else
    // I consider two cases to reduce errors
    {
      if(b>0)
	{
	  x2=-(b+discriminant)/(2*a);
	  x1=c/(a*x2);
	}
      else
	{
	  x1=(-b+discriminant)/(2*a);
	  x2=c/(a*x1);
	}
      return std::make_pair(x1,x2);
    }
}

const char *negativeDiscriminant::what() const throw()
		{
	return "Negative Discriminant";
		}

