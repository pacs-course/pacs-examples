#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP

#include <iostream>
#include <map>
#include <vector>

/// Class for sparse row-oriented matrix.
class sparse_matrix : public std::vector<std::map<unsigned int, double>>
{
private:
  using col_type = std::map<unsigned int, double>;
  using row_type = std::vector<col_type>;

  size_t nnz; ///< Number of nonzero elements.
  size_t m;   ///< Number of nonempty columns.

public:
  /// Default constructor.
  sparse_matrix()
    : nnz(0)
    , m(0)
  {}

  /// Index of non-empty column.
  inline unsigned int
  col_idx(const col_type::iterator &j) const
  {
    return j->first;
  }

  /// Value stored in non-empty column.
  double
  col_val(const col_type::iterator &j) const
  {
    return j->second;
  }

  /// Number of rows.
  inline const size_t
  rows() const
  {
    return size();
  }

  /// Number of columns.
  inline const size_t
  cols() const
  {
    return m;
  }

  /// Recompute sparse matrix properties.
  void
  update_properties();

  /// Convert row-oriented sparse matrix to AIJ format.
  void
  aij(std::vector<double> &      a,
      std::vector<unsigned int> &i,
      std::vector<unsigned int> &j);

  /// Update the entries of a sparse matrix in AIJ format.
  void
  aij_update(std::vector<double> &            a,
             const std::vector<unsigned int> &i,
             const std::vector<unsigned int> &j);

  /// Convert row-oriented sparse matrix to CSR format.
  void
  csr(std::vector<double> &      a,
      std::vector<unsigned int> &col_ind,
      std::vector<unsigned int> &row_ptr);

  /// Update the entries of a sparse matrix in CSR format.
  void
  csr_update(std::vector<double> &            a,
             const std::vector<unsigned int> &col_ind,
             const std::vector<unsigned int> &row_ptr);

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
};

#endif /* SPARSE_MATRIX_HPP */
