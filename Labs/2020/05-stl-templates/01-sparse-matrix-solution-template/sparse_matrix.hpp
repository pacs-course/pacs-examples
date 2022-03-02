#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP

#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <type_traits>
#include <vector>

/// Templated class for sparse row-oriented matrix.
template <class T>
class sparse_matrix_template : public std::vector<std::map<int, T>>
{
private:
  using col_type = std::map<int, T>;
  using row_type = std::vector<col_type>;

public:
  using row_iterator = typename row_type::iterator;
  using col_iterator = typename col_type::iterator;

  /// Index of non-empty column.
  inline int
  col_idx(const col_iterator &j) const
  {
    return (*j).first;
  }

  /// Value stored in non-empty column.
  inline std::remove_pointer_t<T>
  col_val(const col_iterator &j) const
  {
    // Since C++17.
    if constexpr (std::is_pointer_v<T>)
      return *((*j).second);
    else
      return (*j).second;
  }

  size_t m;   ///< number of nonempty columns.
  size_t nnz; ///< number of nonzero elements.

  /// Number of rows.
  inline const size_t
  rows() const
  {
    return this->size();
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
  sparse_matrix_template()
  {
    init();
  };

  /// Stream operator.
  template <class U>
  friend std::ostream &
  operator<<(std::ostream &stream, sparse_matrix_template<U> &M);

  /// Convert row-oriented sparse matrix to AIJ format, with shift.
  void
  aij(std::vector<std::remove_pointer_t<T>> &a,
      std::vector<int> &                     i,
      std::vector<int> &                     j,
      int                                    base);

  /// Convert row-oriented sparse matrix to AIJ format.
  void
  aij(std::vector<std::remove_pointer_t<T>> &a,
      std::vector<int> &                     i,
      std::vector<int> &                     j)
  {
    aij(a, i, j, 0);
  };

  /// Update the entries of a sparse matrix in AIJ format, with shift.
  void
  aij_update(std::vector<std::remove_pointer_t<T>> &a,
             const std::vector<int> &               i,
             const std::vector<int> &               j,
             int                                    base);

  /// Update the entries of a sparse matrix in AIJ format.
  void
  aij_update(std::vector<std::remove_pointer_t<T>> &a,
             const std::vector<int> &               i,
             const std::vector<int> &               j)
  {
    aij_update(a, i, j, 0);
  };

  /// Convert row-oriented sparse matrix to CSR format with shift.
  void
  csr(std::vector<std::remove_pointer_t<T>> &a,
      std::vector<int> &                     col_ind,
      std::vector<int> &                     row_ptr,
      int                                    base);

  /// Convert row-oriented sparse matrix to CSR format.
  void
  csr(std::vector<std::remove_pointer_t<T>> &a,
      std::vector<int> &                     col_ind,
      std::vector<int> &                     row_ptr)
  {
    csr(a, col_ind, row_ptr, 0);
  };

  /// Update the entries of a sparse matrix in CSR format, with shift.
  void
  csr_update(std::vector<std::remove_pointer_t<T>> &a,
             const std::vector<int> &               col_ind,
             const std::vector<int> &               row_ptr,
             int                                    base);

  /// Update the entries of a sparse matrix in CSR format.
  void
  csr_update(std::vector<std::remove_pointer_t<T>> &a,
             const std::vector<int> &               col_ind,
             const std::vector<int> &               row_ptr)
  {
    csr_update(a, col_ind, row_ptr, 0);
  };
};

template <class T>
void
sparse_matrix_template<T>::init()
{
  nnz = 0;
  m   = 0;
}

template <class T>
void
sparse_matrix_template<T>::update_properties()
{
  typename sparse_matrix_template<T>::col_iterator j;
  nnz = 0;
  m   = 0;
  for (size_t i = 0; i < this->size(); ++i)
    {
      if ((*this)[i].size() > 0)
        {
          nnz += (*this)[i].size();

          // std::map are sorted, so we just need to get the index of
          // the last element.
          size_t tmp = (*this)[i].rbegin()->first + 1;

          m = std::max(m, tmp);
        }
    }
}


template <class T>
std::ostream &
operator<<(std::ostream &stream, sparse_matrix_template<T> &M)
{
  typename sparse_matrix_template<T>::col_iterator j;

  M.update_properties();
  stream << "nrows = " << M.rows() << "; ncols = " << M.cols();
  stream << "; nnz = " << M.nnz << ";" << std::endl;
  stream << "mat = [";
  for (size_t i = 0; i < M.size(); ++i)
    {
      if (M[i].size())
        for (j = M[i].begin(); j != M[i].end(); ++j)
          {
            stream << i + 1 << ", " << M.col_idx(j) + 1 << ", ";
            stream << std::setprecision(17) << M.col_val(j) << ";"
                   << std::endl;
          }
    }
  stream << "];" << std::endl;
  return stream;
}

template <class T>
void
sparse_matrix_template<T>::aij(
  std::vector<std::remove_pointer_t<T>> &a,
  std::vector<int> &                     i,
  std::vector<int> &                     j,
  int                                    base)
{
  update_properties();
  a.resize(nnz);
  i.resize(nnz);
  j.resize(nnz);
  int                                              idx = 0;
  typename sparse_matrix_template<T>::col_iterator jj;

  for (size_t ii = 0; ii < this->size(); ++ii)
    if ((*this)[ii].size())
      for (jj = (*this)[ii].begin(); jj != (*this)[ii].end(); ++jj)
        {
          i[idx] = ii + base;
          j[idx] = col_idx(jj) + base;
          a[idx] = col_val(jj);
          ++idx;
        }
}

template <class T>
void
sparse_matrix_template<T>::aij_update(
  std::vector<std::remove_pointer_t<T>> &a,
  const std::vector<int> &               i,
  const std::vector<int> &               j,
  int                                    base)
{
  size_t                                           n = i.size();
  typename sparse_matrix_template<T>::col_iterator jj;
  a.resize(n);

  for (size_t ii = 0; ii < n; ++ii)
    a[ii] = col_val(((*this)[i[ii] - base]).find(j[ii] - base));
}

template <class T>
void
sparse_matrix_template<T>::csr(
  std::vector<std::remove_pointer_t<T>> &a,
  std::vector<int> &                     col_ind,
  std::vector<int> &                     row_ptr,
  int                                    base)
{
  update_properties();
  a.resize(nnz);
  col_ind.resize(nnz);
  row_ptr.resize(rows() + 1);
  int                                              idx = 0;
  int                                              idr = 0;
  typename sparse_matrix_template<T>::col_iterator jj;

  for (size_t ii = 0; ii < this->size(); ++ii)
    {
      row_ptr[idr] = idx + base;
      if ((*this)[ii].size() > 0)
        {
          for (jj = (*this)[ii].begin(); jj != (*this)[ii].end();
               ++jj)
            {
              col_ind[idx] = col_idx(jj) + base;
              a[idx]       = col_val(jj);
              ++idx;
            }
        }
      ++idr;
    }
  std::fill(row_ptr.begin() + idr, row_ptr.end(), nnz + base);
}

template <class T>
void
sparse_matrix_template<T>::csr_update(
  std::vector<std::remove_pointer_t<T>> &a,
  const std::vector<int> &               col_ind,
  const std::vector<int> &               row_ptr,
  int                                    base)
{
  size_t ni = row_ptr.size();
  size_t nj = col_ind.size();
  a.clear();
  a.reserve(nj);

  std::cout << " ni = " << ni << std::endl;
  for (size_t in = 0; in < ni - 1; ++in)
    for (size_t jn = row_ptr[in] - base; jn < row_ptr[in + 1] - base;
         ++jn)
      a.push_back(col_val(((*this)[in]).find(col_ind[jn] - base)));
}


/// Sparse row-oriented double* matrix.
using p_sparse_matrix = sparse_matrix_template<double *>;


using double_sparse_matrix = sparse_matrix_template<double>;

/// Sparse row-oriented double matrix.
class sparse_matrix : public double_sparse_matrix
{
public:
  /// Build a p_sparse_matrix whose entries are pointers to given rows
  /// and columns.
  void
  extract_block_pointer(const std::vector<int> &rows,
                        const std::vector<int> &cols,
                        p_sparse_matrix &       out);

  /// Same as extract_block_pointer but keep column indices in output.
  void
  extract_block_pointer_keep_cols(const std::vector<int> &rows,
                                  const std::vector<int> &cols,
                                  p_sparse_matrix &       out);

  /// Set all entries to 0 preserving storage structure.
  void
  reset();

  /// Sparse matrix increment.
  /// Automatically allocates additional entries.
  template <class T>
  void
  operator+=(T &other);

  /// Compute matrix-vector product.
  friend std::vector<double> operator*(sparse_matrix &            M,
                                       const std::vector<double> &x);
};

template <class T>
void
sparse_matrix::operator+=(T &other)
{
  assert(rows() == other.rows());
  assert(cols() == other.cols());

  col_iterator jj;
  for (size_t ii = 0; ii < other.size(); ++ii)
    if (other[ii].size())
      for (jj = other[ii].begin(); jj != other[ii].end(); ++jj)
        (*this)[ii][jj->first] += other.col_val(jj);
}

#endif /* SPARSE_MATRIX_HPP */
