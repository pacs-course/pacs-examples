#ifndef H_INTEGRANDS_HPP
#define H_INTEGRANDS_HPP
#include <cmath>
/*! \file integrands.hpp
  \brief contains the integrand funcctions
 */
const double pi = 2 * std::atan(1.0);
double       fsincos(double const &);
//! Exact solution for testing
double exact(double const &a, const double &b);
#endif
