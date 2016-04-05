#ifndef HAVE_MATRIX_H
#define HAVE_MATRIX_H

#include <vector>
#include <iostream>

class
matrix
{

private :
  
  std::vector<double> data;
  const unsigned int rows;
  const unsigned int cols;

  inline
  unsigned int
  sub2ind (const unsigned int ir,
           const unsigned int jc) const
  { return (ir + jc * rows); };

  double &
  index (unsigned int irow, unsigned int jcol)
  { return data[sub2ind (irow, jcol)]; };

  const double &
  const_index (unsigned int irow, unsigned int jcol) const
  { return data[sub2ind (irow, jcol)]; };

public :

  matrix (unsigned int size)
    : rows (size), cols (size)
  { data.resize (rows * cols, 0.0); };

  matrix (unsigned int rows_,
          unsigned int cols_)
    : rows (rows_), cols (cols_)
  { data.resize (rows * cols, 0.0); };

  matrix (matrix const &) = default;  
  
  unsigned int
  get_rows () const { return rows; }

  unsigned int
  get_cols () const { return cols; }

  double &
  operator() (unsigned int irow, unsigned int jcol)
  { return index (irow, jcol); };

  const double &
  operator()  (unsigned int irow, unsigned int jcol) const
  { return const_index (irow, jcol); };

  const double *
  get_data () const { return &(data[0]); };

  double *
  get_data () { return &(data[0]); };

  /// transposed matrix : B = A'
  matrix
  transpose () const;

  void
  solve (matrix &rhs);

  void
  gauss_seidel
  (const matrix &f,
   matrix &uh,
   const unsigned int maxit = 100,
   const double tol = 1.e-9);
  
};

/// matrix x matrix product : C = A * B
matrix
operator* (const matrix& A, const matrix& B);

#endif
