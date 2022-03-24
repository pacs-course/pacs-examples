#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP

#include <iostream>
#include <map>
#include <vector>

using row_type = std::map<unsigned int, double>;

/// Class for sparse row-oriented matrix.
class sparse_matrix : public std::vector<row_type>
{
public:
  /// Default constructor.
  sparse_matrix();

  /// Recompute sparse matrix properties.
  void
  update_properties();

  /// Convert row-oriented sparse matrix to AIJ format.
  void
  aij(std::vector<double> &      a,
      std::vector<unsigned int> &i,
      std::vector<unsigned int> &j);

  /// Convert row-oriented sparse matrix to CSR format.
  void
  csr(std::vector<double> &      val,
      std::vector<unsigned int> &col_ind,
      std::vector<unsigned int> &row_ptr);

  /// Set all entries to 0 preserving storage structure.
  void
  reset();

  /// Sparse matrix sum.
  /// Automatically allocates additional entries.
  void
  operator+=(sparse_matrix &other);

  /// Compute matrix-vector product.
  friend std::vector<double>
  operator*(sparse_matrix &M, const std::vector<double> &x);

  /// Stream operator.
  friend std::ostream &
  operator<<(std::ostream &stream, sparse_matrix &M);

private:
  size_t nnz; ///< Number of nonzero elements.
  size_t m;   ///< Number of nonempty columns.
};

#endif /* SPARSE_MATRIX_HPP */
