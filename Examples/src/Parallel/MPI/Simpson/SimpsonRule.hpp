/*
 * SimpsonRule.hpp
 *
 *  Created on: Aug 11, 2022
 *      Author: forma
 */

#ifndef AMSC_EXAMPLES_EXAMPLES_SRC_PARALLEL_MPI_SIMPSON_SIMPSONRULE_HPP_
#define AMSC_EXAMPLES_EXAMPLES_SRC_PARALLEL_MPI_SIMPSON_SIMPSONRULE_HPP_
#include <functional>
//! I use inline to avoid the need to create a source file. Definition is here
/*!
 * Computes the simpson composite quadrature rule between a and b using n intervals
 */
inline double SimpsonRule(std::function<double (double const &)> const & f,
                          double a, double b, unsigned int n)
{
  double integral{0.};
  double h=(b-a)/n;
  for (auto i= 0u; i< n;++i)
    {
      integral+=(f(a+i*h)+4.*f(a+(i+0.5)*h)+f(a+(i+1.)*h));
    }
  return (h/6.)*integral;
}


#endif /* AMSC_EXAMPLES_EXAMPLES_SRC_PARALLEL_MPI_SIMPSON_SIMPSONRULE_HPP_ */
