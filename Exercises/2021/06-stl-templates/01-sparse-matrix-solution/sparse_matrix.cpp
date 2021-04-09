#include "sparse_matrix.hpp"

#include <cassert>
#include <cmath>
#include <iomanip>

void
sparse_matrix::update_properties()
{
  nnz = 0;
  m   = 0;

  for (size_t i = 0; i < size(); ++i)
    {
      if (!(*this)[i].empty())
        {
          nnz += (*this)[i].size();

          // std::map are sorted, so we just need to get the index of
          // the last element.
          const size_t tmp = (*this)[i].rbegin()->first + 1;

          m = std::max(m, tmp);
        }
    }
}


std::ostream &
operator<<(std::ostream &stream, sparse_matrix &M)
{
  M.update_properties();
  stream << "n_rows = " << M.rows() << "; n_cols = " << M.cols()
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


void
sparse_matrix::aij(std::vector<double> &      a,
                   std::vector<unsigned int> &i,
                   std::vector<unsigned int> &j)
{
  update_properties();
  a.resize(nnz);
  i.resize(nnz);
  j.resize(nnz);

  unsigned int idx = 0;

  for (size_t ii = 0; ii < size(); ++ii)
    {
      for (auto jj = (*this)[ii].begin(); jj != (*this)[ii].end(); ++jj)
        {
          i[idx] = ii;
          j[idx] = col_idx(jj);
          a[idx] = col_val(jj);
          ++idx;
        }
    }
}


void
sparse_matrix::aij_update(std::vector<double> &            a,
                          const std::vector<unsigned int> &i,
                          const std::vector<unsigned int> &j)
{
  const size_t n = i.size();

  a.resize(n);

  for (size_t entry = 0; entry < n; ++entry)
    {
      a[entry] = col_val(((*this)[i[entry]]).find(j[entry]));
    }
}


void
sparse_matrix::csr(std::vector<double> &      a,
                   std::vector<unsigned int> &col_ind,
                   std::vector<unsigned int> &row_ptr)
{
  update_properties();
  a.resize(nnz);
  col_ind.resize(nnz);
  row_ptr.resize(rows() + 1);

  unsigned int idx = 0;
  unsigned int idr = 0;

  for (size_t ii = 0; ii < size(); ++ii)
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


void
sparse_matrix::csr_update(std::vector<double> &            a,
                          const std::vector<unsigned int> &col_ind,
                          const std::vector<unsigned int> &row_ptr)
{
  size_t ni = row_ptr.size();
  size_t nj = col_ind.size();
  a.clear();
  a.reserve(nj);

  for (size_t in = 0; in < ni - 1; ++in)
    for (unsigned int jn = row_ptr[in]; jn < row_ptr[in + 1]; ++jn)
      a.push_back(col_val(((*this)[in]).find(col_ind[jn])));
}


void
sparse_matrix::operator+=(sparse_matrix &other)
{
  assert(rows() == other.rows());
  assert(cols() == other.cols());

  for (size_t ii = 0; ii < other.size(); ++ii)
    for (auto jj = other[ii].begin(); jj != other[ii].end(); ++jj)
      (*this)[ii][jj->first] += other.col_val(jj);
}

void
sparse_matrix::reset()
{
  for (auto ii : *this)
    for (auto &jj : ii)
      jj.second = 0.0;
}

std::vector<double>
operator*(sparse_matrix &M, const std::vector<double> &x)
{
  std::vector<double> y(M.rows(), 0.0);

  for (unsigned int i = 0; i < M.size(); ++i)
    for (auto j = M[i].begin(); j != M[i].end(); ++j)
      y[i] += M.col_val(j) * x[M.col_idx(j)];

  return y;
}
