/*
 * ModelBasis.cpp
 *
 *  Created on: Jan 24, 2019
 *      Author: forma
 */
#include <PolynomialBasis.hpp>

//! this is used only internally so I use a special namespace
/*!
 *  I could have used an unnamed namespace
 */
namespace internal
{
  //! For standard monomial basis
  inline double POW(const double & x, const unsigned int N)
    {
      return (N==0u) ? 1.0: x*POW(x,N-1);
    }
  //! Derivative of \f$ x^N \f$
  inline double POWDer(const double &x, const unsigned int N)
  {
    return static_cast<double>(N)*POW(x,N-1);
  }
}

LinearAlgebra::PolynomialMonomialBasisFunction::PolynomialMonomialBasisFunction (
    std::size_t n)
{
  this->setFunctions(n);
}

LinearAlgebra::PolynomialMonomialBasisFunction::Vector
LinearAlgebra::PolynomialMonomialBasisFunction::eval (const double& x) const
{
  Vector a;
  a.resize(this->size());
  size_t i=0;
  // fill vector with values
  for (auto f:this->M_basisFunctions) a(i++,0)=f(x);
  return a;
}

void
LinearAlgebra::PolynomialMonomialBasisFunction::setFunctions (std::size_t n)
{
  this->M_basisFunctions.resize(n+1);
  this->M_derivatives.resize(n+1);

  for  (std::size_t i=0; i<=n;++i)
    {
      this->M_basisFunctions[i] = [i](double const & x){return internal::POW(x,i);};
      this->M_derivatives[i]    = [i](double const & x){return internal::POWDer(x,i);};
    }
}

// @todo There is a duplication of code with eval. It can be done better.
LinearAlgebra::PolynomialMonomialBasisFunction::Vector
LinearAlgebra::PolynomialMonomialBasisFunction::derivatives (
    const double& x) const
{
  Vector a;
  a.resize(this->size());
  size_t i=0;
  // fill vector with values
  for (auto f:this->M_derivatives) a(i++,0)=f(x);
  return a;
}
