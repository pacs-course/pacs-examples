#ifndef HAVE_MATRIX_H
#define HAVE_MATRIX_H

#include <iostream>
#include <vector>

class matrix
{
public:
  matrix(size_t size)
    : rows(size)
    , cols(size)
  {
    data.resize(rows * cols, 0.0);
  };

  matrix(size_t rows_, size_t cols_)
    : rows(rows_)
    , cols(cols_)
  {
    data.resize(rows * cols, 0.0);
  };

  matrix(const matrix &) = default;

  size_t
  get_rows() const
  {
    return rows;
  }

  size_t
  get_cols() const
  {
    return cols;
  }

  double &
  operator()(size_t irow, size_t jcol)
  {
    return value(irow, jcol);
  };

  const double &
  operator()(size_t irow, size_t jcol) const
  {
    return value(irow, jcol);
  };

  const double *
  get_data() const
  {
    return &(data[0]);
  };

  double *
  get_data()
  {
    return &(data[0]);
  };

  /// transposed matrix : B = A'
  matrix
  transpose() const;

  void
  solve(matrix &rhs);

  void
  factorize();

private:
  std::vector<double> data;
  std::vector<int>    p;
  const size_t        rows;
  const size_t        cols;

  inline size_t
  sub2ind(const size_t ir, const size_t jc) const
  {
    return (ir + jc * rows);
  };

  double &
  value(size_t irow, size_t jcol)
  {
    return data[sub2ind(irow, jcol)];
  };

  const double &
  value(size_t irow, size_t jcol) const
  {
    return data[sub2ind(irow, jcol)];
  };

  bool factorized;
};

/// matrix x matrix product : C = A * B
matrix
operator*(const matrix &A, const matrix &B);

#endif
