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
class sparse_matrix_template : public std::vector<std::map<unsigned int, T>>
{
private:
  using col_type = std::map<unsigned int, T>;
  using row_type = std::vector<col_type>;

  size_t nnz; ///< Number of nonzero elements.
  size_t m;   ///< Number of nonempty columns.

public:
  /// Default constructor.
  sparse_matrix_template()
    : nnz(0)
    , m(0)
  {}

  /// Index of non-empty column.
  inline unsigned int
  col_idx(const typename col_type::iterator &j) const
  {
    return j->first;
  }

  /// Value stored in non-empty column.
  inline std::remove_pointer_t<T>
  col_val(const typename col_type::iterator &j) const
  {
    // Since C++17.
    if constexpr (std::is_pointer_v<T>)
      return *(j->second);
    else
      return j->second;
  }

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

  /// Recompute sparse matrix properties.
  void
  update_properties();

  /// Stream operator.
  template <class U>
  friend std::ostream &
  operator<<(std::ostream &stream, sparse_matrix_template<U> &M);

  /// Convert row-oriented sparse matrix to AIJ format.
  void
  aij(std::vector<std::remove_pointer_t<T>> &a,
      std::vector<unsigned int> &            i,
      std::vector<unsigned int> &            j);

  /// Update the entries of a sparse matrix in AIJ format.
  void
  aij_update(std::vector<std::remove_pointer_t<T>> &a,
             const std::vector<unsigned int> &      i,
             const std::vector<unsigned int> &      j);

  /// Convert row-oriented sparse matrix to CSR format.
  void
  csr(std::vector<std::remove_pointer_t<T>> &a,
      std::vector<unsigned int> &            col_ind,
      std::vector<unsigned int> &            row_ptr);

  /// Update the entries of a sparse matrix in CSR format.
  void
  csr_update(std::vector<std::remove_pointer_t<T>> &a,
             const std::vector<unsigned int> &      col_ind,
             const std::vector<unsigned int> &      row_ptr);
};

template <class T>
void
sparse_matrix_template<T>::update_properties()
{
  nnz = 0;
  m   = 0;

  for (size_t i = 0; i < this->size(); ++i)
    {
      if (!(*this)[i].empty())
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
  M.update_properties();
  stream << "nrows = " << M.rows() << "; ncols = " << M.cols()
         << "; nnz = " << M.nnz << ";" << std::endl;
  stream << "mat = [";
  for (size_t i = 0; i < M.size(); ++i)
    {
      for (auto j = M[i].begin(); j != M[i].end(); ++j)
        {
          stream << i + 1 << ", " << M.col_idx(j) + 1 << ", ";
          stream << std::setprecision(17) << M.col_val(j) << ";" << std::endl;
        }
    }
  stream << "];" << std::endl;
  return stream;
}

template <class T>
void
sparse_matrix_template<T>::aij(std::vector<std::remove_pointer_t<T>> &a,
                               std::vector<unsigned int> &            i,
                               std::vector<unsigned int> &            j)
{
  update_properties();
  a.resize(nnz);
  i.resize(nnz);
  j.resize(nnz);

  unsigned int idx = 0;

  for (size_t ii = 0; ii < this->size(); ++ii)
    for (auto jj = (*this)[ii].begin(); jj != (*this)[ii].end(); ++jj)
      {
        i[idx] = ii;
        j[idx] = col_idx(jj);
        a[idx] = col_val(jj);
        ++idx;
      }
}

template <class T>
void
sparse_matrix_template<T>::aij_update(std::vector<std::remove_pointer_t<T>> &a,
                                      const std::vector<unsigned int> &      i,
                                      const std::vector<unsigned int> &      j)
{
  size_t n = i.size();
  a.resize(n);

  for (size_t entry = 0; entry < n; ++entry)
    a[entry] = col_val(((*this)[i[entry]]).find(j[entry]));
}

template <class T>
void
sparse_matrix_template<T>::csr(std::vector<std::remove_pointer_t<T>> &a,
                               std::vector<unsigned int> &            col_ind,
                               std::vector<unsigned int> &            row_ptr)
{
  update_properties();
  a.resize(nnz);
  col_ind.resize(nnz);
  row_ptr.resize(rows() + 1);
  unsigned int idx = 0;
  unsigned int idr = 0;

  for (size_t ii = 0; ii < this->size(); ++ii)
    {
      row_ptr[idr] = idx;
      for (auto jj = (*this)[ii].begin(); jj != (*this)[ii].end(); ++jj)
        {
          col_ind[idx] = col_idx(jj);
          a[idx]       = col_val(jj);
          ++idx;
        }
      ++idr;
    }
  std::fill(row_ptr.begin() + idr, row_ptr.end(), nnz);
}

template <class T>
void
sparse_matrix_template<T>::csr_update(std::vector<std::remove_pointer_t<T>> &a,
                                      const std::vector<unsigned int> &col_ind,
                                      const std::vector<unsigned int> &row_ptr)
{
  size_t ni = row_ptr.size();
  size_t nj = col_ind.size();
  a.clear();
  a.reserve(nj);

  for (size_t in = 0; in < ni - 1; ++in)
    for (size_t jn = row_ptr[in]; jn < row_ptr[in + 1]; ++jn)
      a.push_back(col_val(((*this)[in]).find(col_ind[jn])));
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
  extract_block_pointer(const std::vector<unsigned int> &rows,
                        const std::vector<unsigned int> &cols,
                        p_sparse_matrix &                out);

  /// Same as extract_block_pointer but keep column indices in output.
  void
  extract_block_pointer_keep_cols(const std::vector<unsigned int> &rows,
                                  const std::vector<unsigned int> &cols,
                                  p_sparse_matrix &                out);

  /// Set all entries to 0 preserving storage structure.
  void
  reset();

  /// Sparse matrix sum.
  /// Automatically allocates additional entries.
  template <class T>
  void
  operator+=(T &other);

  /// Compute matrix-vector product.
  friend std::vector<double>
  operator*(sparse_matrix &M, const std::vector<double> &x);
};

template <class T>
void
sparse_matrix::operator+=(T &other)
{
  assert(rows() == other.rows());
  assert(cols() == other.cols());

  for (size_t ii = 0; ii < other.size(); ++ii)
    for (auto jj = other[ii].begin(); jj != other[ii].end(); ++jj)
      (*this)[ii][jj->first] += other.col_val(jj);
}

#endif /* SPARSE_MATRIX_HPP */
