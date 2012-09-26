/*
 * QuadraticRoot.hpp
 *
 *  Created on: 22 Aug 2011
 *      Author: formaggia
 */

#ifndef QUADRATICROOT_HPP_
#define QUADRATICROOT_HPP_
#include <exception>
// This is a trick to allow with -DREAL=XXX to 
// set the float number type
#ifndef REAL
#define REAL double
#endif
typedef REAL Real;

//! Negative discriminant
/*!
  An exception that will be trown if the discriminant is negative
 */
class negativeDiscriminant: public std::exception
{
public:
	virtual const char * what() const throw();
};

//! Computer the root of a quadratic polynomial using basic rule
std::pair<Real,Real>
quadraticRoot_simple(
		Real const & a, Real const & b, Real const & c)
		throw(negativeDiscriminant);


//! Computer the root of a quadratic polynomial using a smarter rule
std::pair<Real,Real>
quadraticRoot(
		Real const & a, Real const & b, Real const & c)
		throw(negativeDiscriminant);
#endif /* QUADRATICROOT_HPP_ */
