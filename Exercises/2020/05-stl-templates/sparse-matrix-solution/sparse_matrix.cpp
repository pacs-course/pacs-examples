#include "sparse_matrix.hpp"

#include <cassert>
#include <cmath>
#include <iomanip>

void
sparse_matrix::init()
{
  nnz = 0;
  m   = 0;
}

void
sparse_matrix::update_properties()
{
  sparse_matrix::col_iterator j;
  nnz = 0;
  m   = 0;
  for (size_t i = 0; i < size(); ++i)
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


std::ostream &
operator<<(std::ostream &stream, sparse_matrix &M)
{
  sparse_matrix::col_iterator j;

  M.update_properties();
  stream << "n_rows = " << M.rows() << "; n_cols = " << M.cols();
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


void
sparse_matrix::aij(std::vector<double> &a,
                   std::vector<int> &   i,
                   std::vector<int> &   j,
                   int                  base)
{
  update_properties();
  a.resize(nnz);
  i.resize(nnz);
  j.resize(nnz);
  int                         idx = 0;
  sparse_matrix::col_iterator jj;

  for (size_t ii = 0; ii < size(); ++ii)
    if ((*this)[ii].size())
      for (jj = (*this)[ii].begin(); jj != (*this)[ii].end(); ++jj)
        {
          i[idx] = ii + base;
          j[idx] = col_idx(jj) + base;
          a[idx] = col_val(jj);
          ++idx;
        }
}


void
sparse_matrix::aij_update(std::vector<double> &   a,
                          const std::vector<int> &i,
                          const std::vector<int> &j,
                          int                     base)
{
  size_t                      n = i.size();
  sparse_matrix::col_iterator jj;
  a.resize(n);

  for (size_t ii = 0; ii < n; ++ii)
    a[ii] = col_val(((*this)[i[ii] - base]).find(j[ii] - base));
}


void
sparse_matrix::csr(std::vector<double> &a,
                   std::vector<int> &   col_ind,
                   std::vector<int> &   row_ptr,
                   int                  base)
{
  update_properties();
  a.resize(nnz);
  col_ind.resize(nnz);
  row_ptr.resize(rows() + 1);
  int                         idx = 0;
  int                         idr = 0;
  sparse_matrix::col_iterator jj;

  for (size_t ii = 0; ii < size(); ++ii)
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


void
sparse_matrix::csr_update(std::vector<double> &   a,
                          const std::vector<int> &col_ind,
                          const std::vector<int> &row_ptr,
                          int                     base)
{
  size_t ni = row_ptr.size();
  size_t nj = col_ind.size();
  a.clear();
  a.reserve(nj);

  std::cout << " ni = " << ni << std::endl;
  for (size_t in = 0; in < ni - 1; ++in)
    for (int jn = row_ptr[in] - base; jn < row_ptr[in + 1] - base;
         ++jn)
      a.push_back(col_val(((*this)[in]).find(col_ind[jn] - base)));
}


void
sparse_matrix::operator+=(sparse_matrix &other)
{
  assert(rows() == other.rows());
  assert(cols() == other.cols());

  col_iterator jj;
  for (size_t ii = 0; ii < other.size(); ++ii)
    if (other[ii].size())
      for (jj = other[ii].begin(); jj != other[ii].end(); ++jj)
        (*this)[ii][jj->first] += other.col_val(jj);
}

void
sparse_matrix::reset()
{
  sparse_matrix::row_iterator ii;
  sparse_matrix::col_iterator jj;
  for (ii = begin(); ii != end(); ++ii)
    for (jj = (*ii).begin(); jj != (*ii).end(); ++jj)
      (*jj).second = 0.0;
}

std::vector<double> operator*(sparse_matrix &            M,
                              const std::vector<double> &x)
{
  std::vector<double> y(M.rows(), 0.0);

  sparse_matrix::col_iterator j;
  for (unsigned int i = 0; i < M.size(); ++i)
    if (M[i].size())
      for (j = M[i].begin(); j != M[i].end(); ++j)
        y[i] += M.col_val(j) * x[M.col_idx(j)];

  return y;
}
