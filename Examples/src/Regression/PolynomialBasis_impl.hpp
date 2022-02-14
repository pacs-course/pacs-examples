/*
 * ModelBasis.cpp
 *
 *  Created on: Jan 24, 2019
 *      Author: forma
 */
#ifndef HH_POLINOMIALBASIS_IMPL_HH
#define HH_POLINOMIALBASIS_IMPL_HH

// This include is not needed. But is is useful to check syntax when using IDE
// like eclipse (if not it will find lots of false errors!)
// It is safe because of the header guard in PolynomialBasis.hpp
//
#include <PolynomialBasis.hpp>

//! this is used only internally so I use a special namespace
/*!
 *  I could have used an unnamed namespace
 */
namespace internal
{
//! For standard monomial basis
constexpr double
POW(const double &x, const unsigned int N)
{
  return (N == 0u) ? 1.0 : x * POW(x, N - 1);
}
//! Derivative of \f$ x^N \f$
constexpr double
POWDer(const double &x, const unsigned int N)
{
  if(N == 0u)
    return 0.0;
  else
    return N * POW(x, N - 1);
}
} // namespace internal

template <LinearAlgebra::LinearAlgebraLibrary L>
LinearAlgebra::PolynomialMonomialBasisFunction<
  L>::PolynomialMonomialBasisFunction(std::size_t n)
{
  this->setFunctions(n);
}

template <LinearAlgebra::LinearAlgebraLibrary L>
typename LinearAlgebra::PolynomialMonomialBasisFunction<L>::Vector
LinearAlgebra::PolynomialMonomialBasisFunction<L>::eval(const double &x) const
{
  Vector a;
  a.resize(this->size());
  size_t i = 0;
  // fill vector with values
  for(auto f : this->M_basisFunctions)
    a(i++, 0) = f(x);
  return a;
}

template <LinearAlgebra::LinearAlgebraLibrary L>
void
LinearAlgebra::PolynomialMonomialBasisFunction<L>::setFunctions(std::size_t n)
{
  this->M_basisFunctions.resize(n + 1);
  this->M_derivatives.resize(n + 1);

  for(std::size_t i = 0; i <= n; ++i)
    {
      this->M_basisFunctions[i] = [i](double const &x) {
        return internal::POW(x, i);
      };
      this->M_derivatives[i] = [i](double const &x) {
        return internal::POWDer(x, i);
      };
    }
}

// @todo There is a duplication of code with eval. It can be done better.
template <LinearAlgebra::LinearAlgebraLibrary L>
typename LinearAlgebra::PolynomialMonomialBasisFunction<L>::Vector
LinearAlgebra::PolynomialMonomialBasisFunction<L>::derivatives(
  const double &x) const
{
  Vector a;
  a.resize(this->size());
  size_t i = 0;
  // fill vector with values
  for(auto f : this->M_derivatives)
    a(i++, 0) = f(x);
  return a;
}
#endif
