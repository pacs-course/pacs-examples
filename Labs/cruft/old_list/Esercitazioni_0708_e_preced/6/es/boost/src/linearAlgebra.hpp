#ifndef LINEARALGEBRA_HPP
#define LINEARALGEBRA_HPP 1

#include <cassert> //!< C-style assertions
#include <iostream> //!< Input-output from standard channels
#include <cmath> //!< Mathematical functions
#include "boost/numeric/ublas/vector.hpp" //!< Boost vector
#include "boost/numeric/ublas/matrix.hpp" //!< Boost matrix
#include "boost/numeric/ublas/io.hpp" //!< Input-output for UBLAS objects

namespace LinearAlgebra {
  //! Alias for long namespace name
  namespace UBLAS=boost::numeric::ublas;
  //! Full matrix type
  typedef UBLAS::matrix<double> Matrix;
  //! Full vector type
  typedef UBLAS::vector<double> Vector;
  //! Compute the Frobenius norm for a matrix
  double FrobeniusNorm(const Matrix& /* A */);
  //! Compute the psi norm for a matrix
  double PsiNorm(const Matrix& /* A */);
  //! Extract the main diagonal
  Vector diag(const Matrix& /* A */);

} // namespace LinearAlgebra

#endif
