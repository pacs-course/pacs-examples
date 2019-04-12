#include "sparse_matrix_simplified.h"

void
sparse_matrix::init () { nnz = 0; m =0; };

void
sparse_matrix::set_properties ()
{
  sparse_matrix::col_iterator j;
  nnz = 0; m = 0;
  for (size_t i = 0; i < this->size (); ++i)
    {
      nnz    += (*this)[i].size ();
      for (j = (*this)[i].begin (); j != (*this)[i].end (); ++j)
        {
          size_t tmp = (*this).col_idx (j) + 1;
          m       = m < tmp ? tmp : m;
        }
    }
}


std::ostream &
operator<< (std::ostream &stream,
            sparse_matrix &sp)
{
  sparse_matrix::col_iterator j;

  sp.set_properties ();
  stream << "nrows = " << sp.rows () << "; ncols = " << sp.cols ();
  stream << "; nnz = " << sp.nnz << ";" << std::endl;
  stream << "mat = spconvert ([" ;
  for (size_t i = 0; i < sp.size (); ++i)
    {
      if (sp[i].size ())
        for (j = sp[i].begin (); j != sp[i].end (); ++j)
          {
            stream << i+1 << ", " << sp.col_idx (j) + 1 << ", ";
            stream  << std::setprecision (17)
                    << sp.col_val (j) << ";" << std::endl;
          }
    }
  stream << "]);" << std::endl ;
  return stream;
}


void
sparse_matrix::aij (std::vector<double> &a,
                    std::vector<int> &i,
                    std::vector<int> &j,
                    int base)
{
  this->set_properties ();
  a.resize (nnz); i.resize (nnz); j.resize (nnz);
  int idx = 0;
  sparse_matrix::col_iterator jj;

  for (size_t ii = 0; ii < this->size (); ++ii)
    if ((*this)[ii].size ())
      for (jj  = (*this)[ii].begin (); jj != (*this)[ii].end (); ++jj)
        {
          i[idx] = ii+base;
          j[idx] = this->col_idx (jj)+base;
          a[idx] = this->col_val (jj);
          idx++;
        }
}


void
sparse_matrix::aij_update (std::vector<double> &a,
                           const std::vector<int> &i,
                           const std::vector<int> &j,
                           int base)
{
  size_t n = i.size ();
  sparse_matrix::col_iterator jj;
  a.resize (n);

  for (size_t ii = 0; ii < n; ++ii)
    a[ii] =
      this->col_val (((*this)[i[ii] - base]).find (j[ii] - base));

}


void
sparse_matrix::csr (std::vector<double> &a,
                    std::vector<int> &col_ind,
                    std::vector<int> &row_ptr,
                    int base)
{
  this->set_properties ();
  a.resize (nnz); col_ind.resize (nnz);
  row_ptr.resize (this->rows () + 1);
  int idx = 0;
  int idr = 0;
  sparse_matrix::col_iterator jj;

  for (size_t ii = 0; ii < this->size (); ++ii)
    {
      row_ptr[idr] = idx + base;
      if ((*this)[ii].size () > 0)
        {
          for (jj  = (*this)[ii].begin (); jj != (*this)[ii].end (); ++jj)
            {
              col_ind[idx] = this->col_idx (jj) + base;
              a[idx] = this->col_val (jj);
              idx++;
            }
        }
      idr++;
    }
  std::fill (row_ptr.begin () + idr, row_ptr.end (), nnz + base);
}


void
sparse_matrix::csr_update (std::vector<double> &a,
                           const std::vector<int> &col_ind,
                           const std::vector<int> &row_ptr,
                           int base)
{
  size_t ni = row_ptr.size ();
  size_t nj = col_ind.size ();
  a.clear ();
  a.reserve (nj);

  std::cout << " ni = " << ni << std::endl;
  for (size_t in = 0; in < ni - 1; ++in)
    for (size_t jn = row_ptr[in] - base; jn < row_ptr[in+1] - base; ++jn)
      a.push_back (col_val (((*this)[in]).find (col_ind[jn] - base)));

}


void
sparse_matrix::operator+= (sparse_matrix &adm)
{

  assert (this->rows () == adm.rows ());
  assert (this->cols () == adm.cols ());

  col_iterator jj;
  for (size_t ii = 0; ii < adm.size (); ++ii)
    if (adm[ii].size ())
      for (jj  = adm[ii].begin (); jj != adm[ii].end (); ++jj)
        (*this)[ii][jj->first] += adm.col_val (jj);
}

void sparse_matrix::reset ()
{
  sparse_matrix::row_iterator ii;
  sparse_matrix::col_iterator jj;
  for (ii = this->begin (); ii != this->end (); ++ii)
    for (jj = (*ii).begin (); jj != (*ii).end (); ++jj)
      (*jj).second = 0.0;
}

std::vector<double>
operator * (sparse_matrix& M, const std::vector<double>& x)
{
  std::vector<double> y (M.rows (), 0.0);
  sparse_matrix::col_iterator j;
  for (unsigned int i = 0; i < M.size (); ++i)
    if (M[i].size ())
      for (j = M[i].begin (); j != M[i].end (); ++j)
        y[i] += M.col_val (j) * x[M.col_idx (j)];

  return y;
}
