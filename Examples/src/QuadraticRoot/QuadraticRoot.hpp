/*
 * QuadraticRoot.hpp
 *
 *  Created on: 22 Aug 2011
 *      Author: formaggia
 */

#ifndef QUADRATICROOT_HPP_
#define QUADRATICROOT_HPP_
#include <complex>
// This is a trick to allow with -DREAL=XXX to 
// set the float number type
#ifndef REAL
#define REAL double
#endif
typedef REAL Real;
using result_type=std::complex<Real>;
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
std::pair<result_type,result_type>
quadraticRoot_simple(
                     Real const & a, Real const & b, Real const & c);


//! Computer the root of a quadratic polynomial using a smarter rule
std::pair<result_type,result_type>
quadraticRoot(
              Real const & a, Real const & b, Real const & c);
#endif /* QUADRATICROOT_HPP_ */
