#ifndef HAVE_SPARSE_MATRIX_SIMPLIFIED_H
#define HAVE_SPARSE_MATRIX_SIMPLIFIED_H 1

#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cassert>

/// Class for sparse row-oriented matrix.
class
sparse_matrix
  : public std::vector< std::map<int, double>>
{
private:

  typedef std::map<int, double>                col_type;
  typedef std::vector< std::map<int, double>>  row_type;

public:

  typedef typename row_type::iterator row_iterator;
  typedef typename col_type::iterator col_iterator;

  /// Index of non-empty column.
  inline int
  col_idx (col_iterator j) const
  { return (*j).first; }

  /// Value stored in non-empty column.
  double
  col_val (col_iterator j) const
  { return (*j).second; }

  size_t m; ///< number of nonempty columns.
  size_t nnz; ///< number of nonzero elements.

  /// Number of rows.
  inline const size_t
  rows () const
  { return this->size (); }

  /// Number of columns.
  inline const size_t
  cols () const
  { return m; }

  /// Init an empty sparse matrix.
  void init ();

  /// Recompute sparse matrix properties.
  void set_properties ();

  /// Default constructor.
  sparse_matrix () {init ();};

  /// Convert row-oriented sparse matrix to AIJ format, with shift.
  void
  aij (std::vector<double> &a,
       std::vector<int> &i,
       std::vector<int> &j,
       int base);

  /// Convert row-oriented sparse matrix to AIJ format.
  void
  aij (std::vector<double> &a,
       std::vector<int> &i,
       std::vector<int> &j)
  { this->aij (a, i, j, 0); };

  /// Update the entries of a sparse matrix in AIJ format, with shift.
  void
  aij_update (std::vector<double> &a,
              const std::vector<int> &i,
              const std::vector<int> &j,
              int base);

  /// Update the entries of a sparse matrix in AIJ format.
  void
  aij_update (std::vector<double> &a,
              const std::vector<int> &i,
              const std::vector<int> &j)
  { this->aij_update (a, i, j, 0); };

  /// Convert row-oriented sparse matrix to CSR format with shift.
  void
  csr (std::vector<double> &a,
       std::vector<int> &col_ind,
       std::vector<int> &row_ptr,
       int base);
    
  /// Convert row-oriented sparse matrix to CSR format.
  void
  csr (std::vector<double> &a,
       std::vector<int> &col_ind,
       std::vector<int> &row_ptr)
  { this->csr (a, col_ind, row_ptr, 0); };

  /// Update the entries of a sparse matrix in CSR format, with shift.
  void
  csr_update (std::vector<double> &a,
              const std::vector<int> &col_ind,
              const std::vector<int> &row_ptr,
              int base);

  /// Update the entries of a sparse matrix in CSR format.
  void
  csr_update (std::vector<double> &a,
              const std::vector<int> &col_ind,
              const std::vector<int> &row_ptr)
  { this->csr_update (a, col_ind, row_ptr, 0); };

  /// Set all entries to 0 preserving storage structure.
  void
  reset ();

  /// Sparse matrix increment. Automatically allocates additional entries.
  void 
  operator+= (sparse_matrix &adm);

  /// Compute matrix-vector product.
  friend std::vector<double>
  operator* (sparse_matrix& M, const std::vector<double>& x);

};

///
std::ostream
&operator<< (std::ostream &, sparse_matrix&);

#endif
