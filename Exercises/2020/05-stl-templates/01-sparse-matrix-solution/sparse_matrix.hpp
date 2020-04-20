#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP

#include <iostream>
#include <map>
#include <vector>

/// Class for sparse row-oriented matrix.
class sparse_matrix : public std::vector<std::map<int, double>>
{
private:
  using col_type = std::map<int, double>;
  using row_type = std::vector<col_type>;

public:
  using row_iterator = row_type::iterator;
  using col_iterator = col_type::iterator;

  /// Index of non-empty column.
  inline int
  col_idx(const col_iterator &j) const
  {
    return (*j).first;
  }

  /// Value stored in non-empty column.
  double
  col_val(const col_iterator &j) const
  {
    return (*j).second;
  }

  size_t m;   ///< number of nonempty columns.
  size_t nnz; ///< number of nonzero elements.

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

  /// Init an empty sparse matrix.
  void
  init();

  /// Recompute sparse matrix properties.
  void
  update_properties();

  /// Default constructor.
  sparse_matrix()
  {
    init();
  };

  /// Convert row-oriented sparse matrix to AIJ format, with shift.
  void
  aij(std::vector<double> &a,
      std::vector<int> &   i,
      std::vector<int> &   j,
      int                  base);

  /// Convert row-oriented sparse matrix to AIJ format.
  void
  aij(std::vector<double> &a,
      std::vector<int> &   i,
      std::vector<int> &   j)
  {
    aij(a, i, j, 0);
  };

  /// Update the entries of a sparse matrix in AIJ format, with shift.
  void
  aij_update(std::vector<double> &   a,
             const std::vector<int> &i,
             const std::vector<int> &j,
             int                     base);

  /// Update the entries of a sparse matrix in AIJ format.
  void
  aij_update(std::vector<double> &   a,
             const std::vector<int> &i,
             const std::vector<int> &j)
  {
    aij_update(a, i, j, 0);
  };

  /// Convert row-oriented sparse matrix to CSR format with shift.
  void
  csr(std::vector<double> &a,
      std::vector<int> &   col_ind,
      std::vector<int> &   row_ptr,
      int                  base);

  /// Convert row-oriented sparse matrix to CSR format.
  void
  csr(std::vector<double> &a,
      std::vector<int> &   col_ind,
      std::vector<int> &   row_ptr)
  {
    csr(a, col_ind, row_ptr, 0);
  };

  /// Update the entries of a sparse matrix in CSR format, with shift.
  void
  csr_update(std::vector<double> &   a,
             const std::vector<int> &col_ind,
             const std::vector<int> &row_ptr,
             int                     base);

  /// Update the entries of a sparse matrix in CSR format.
  void
  csr_update(std::vector<double> &   a,
             const std::vector<int> &col_ind,
             const std::vector<int> &row_ptr)
  {
    csr_update(a, col_ind, row_ptr, 0);
  };

  /// Set all entries to 0 preserving storage structure.
  void
  reset();

  /// Sparse matrix increment.
  /// Automatically allocates additional entries.
  void
  operator+=(sparse_matrix &other);

  /// Compute matrix-vector product.
  friend std::vector<double> operator*(sparse_matrix &            M,
                                       const std::vector<double> &x);

  /// Stream operator.
  friend std::ostream &
  operator<<(std::ostream &stream, sparse_matrix &M);
};

#endif /* SPARSE_MATRIX_HPP */
