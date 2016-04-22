#ifndef HAVE_SPARSE_H
#define HAVE_SPARSE_H

#include <map>
#include "matrix.h"
#include <cassert>
#include <cmath>

class
sparse_matrix
  : public std::vector<std::map<unsigned int, double> >
{
private :

  unsigned int nrows;
  unsigned int ncols;
  double       zero = 0.0;
  
public :

  sparse_matrix (unsigned int size);

  sparse_matrix (unsigned int m, unsigned int n);

  unsigned int
  get_rows () const
  { return nrows; };

  unsigned int
  get_cols () const
  { return ncols; };

  double &
  operator() (unsigned int irow, unsigned int jcol);
  
  const double &
  operator()  (unsigned int irow, unsigned int jcol) const;

  sparse_matrix
  transpose () const;

  void
  gauss_seidel
  (const matrix &f,
   matrix &uh,
   const unsigned int maxit = 100,
   const double tol = 1.e-9);

  void
  solve (sparse_matrix &rhs)
  { assert (false); };

};

sparse_matrix
operator* (const sparse_matrix& A, const sparse_matrix& B);

matrix
operator* (const sparse_matrix& A, const matrix& B);

matrix
operator* (const matrix& A, const sparse_matrix& B);

#endif
