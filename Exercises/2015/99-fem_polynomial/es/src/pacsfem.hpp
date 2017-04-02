#ifndef LINEARALGEBRA_HPP
#define LINEARALGEBRA_HPP 1

#include <cassert> //!< C-style assertions
#include <iostream> //!< Input-output from standard channels
#include <cmath> //!< Mathematical functions
#include "boost/numeric/ublas/vector.hpp" //!< Boost vector
#include "boost/numeric/ublas/matrix.hpp" //!< Boost matrix
#include "boost/numeric/ublas/io.hpp" //!< Input-output for UBLAS classes

#define F77NAME(X) X ## _

//! Alias for long namespace name
namespace UBLAS=boost::numeric::ublas;

//! Floating point type
typedef float Real;

//! Full matrix type
typedef UBLAS::matrix<Real> Matrix;

//! Full vector type
typedef UBLAS::vector<Real> Vector;

#endif
