/*
  Copyright (C) 2011 Carlo de Falco
  This software is distributed under the terms
  the terms of the GNU/GPL licence v3
*/


#ifndef HAVE_SPARSE_H
#define HAVE_SPARSE_H 1

#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cassert>

/// Templated class for sparse row-oriented matrix.
template <class T>
class sparse_matrix_template : public std::vector< std::map<int, T> >
{
private:

  typedef std::map<int, T>                col_type;
  typedef std::vector< std::map<int, T> > row_type;

public:

  typedef typename row_type::iterator row_iterator;
  typedef typename col_type::iterator col_iterator;

  /// Index of non-empty column.
  inline int
  col_idx (col_iterator j) const
  { return (*j).first; }

  /// Value stored in non-empty column.
  virtual double
  col_val (col_iterator j) = 0;

  size_t m; ///< number of nonempty columns.
  size_t nnz; ///< number of nonzero elements.

  /// Number of rows.
  inline const size_t
  rows () const
  {return this->size ();}

  /// Number of columns.
  inline const size_t
  cols () const
  { return m; }

  /// Init an empty sparse matrix.
  void init ();

  /// Recompute sparse matrix properties.
  void set_properties ();

  /// Default constructor.
  sparse_matrix_template () {init ();};

  ///
  template<class U> friend std::ostream
  &operator<< (std::ostream &, sparse_matrix_template<U> &);

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

  /// Convert row-oriented sparse matrix to CRS format with shift.
  void
  csr (std::vector<double> &a,
       std::vector<int> &col_ind,
       std::vector<int> &row_ptr,
       int base);
    
  /// Convert row-oriented sparse matrix to CRS format.
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
};

template<class Y>
void sparse_matrix_template<Y>::init () {nnz = 0; m = 0;}

template<class Y>
void sparse_matrix_template<Y>::set_properties ()
{
  typename sparse_matrix_template<Y>::col_iterator j;
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


template<class Y>
std::ostream & operator<< (std::ostream &stream,
                           sparse_matrix_template<Y> &sp)
{
  typename sparse_matrix_template<Y>::col_iterator j;

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

template<class T>
void sparse_matrix_template<T>::aij (std::vector<double> &a,
                                     std::vector<int> &i,
                                     std::vector<int> &j,
                                     int base)
{
  this->set_properties ();
  a.resize (nnz); i.resize (nnz); j.resize (nnz);
  int idx = 0;
  typename sparse_matrix_template<T>::col_iterator jj;

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

template<class T>
void sparse_matrix_template<T>::aij_update (std::vector<double> &a,
                                            const std::vector<int> &i,
                                            const std::vector<int> &j,
                                            int base)
{
  size_t n = i.size ();
  typename sparse_matrix_template<T>::col_iterator jj;
  a.resize (n);

  for (size_t ii = 0; ii < n; ++ii)
    a[ii] =
      this->col_val (((*this)[i[ii] - base]).find (j[ii] - base));

}

template<class T>
void sparse_matrix_template<T>::csr (std::vector<double> &a,
                                     std::vector<int> &col_ind,
                                     std::vector<int> &row_ptr,
                                     int base)
{
  this->set_properties ();
  a.resize (nnz); col_ind.resize (nnz);
  row_ptr.resize (this->rows () + 1);
  int idx = 0;
  int idr = 0;
  typename sparse_matrix_template<T>::col_iterator jj;

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

template<class T>
void sparse_matrix_template<T>::csr_update (std::vector<double> &a,
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


typedef  sparse_matrix_template<double> double_sparse_matrix;
typedef  sparse_matrix_template<double*> double_p_sparse_matrix;

/// Sparse row-oriented double* matrix.
class
p_sparse_matrix : public double_p_sparse_matrix
{
public :
  double
  col_val (double_p_sparse_matrix::col_iterator j)
  {return *((*j).second);}
};

/// Sparse row-oriented double matrix.
class sparse_matrix : public double_sparse_matrix
{
public :

  double
  col_val (double_sparse_matrix::col_iterator j)
  {return (*j).second;}

  /// Build a p_sparse_matrix whose entries are pointers to given rows and columns.
  void
  extract_block_pointer (const std::vector<int> &rows,
                         const std::vector<int> &cols,
                         p_sparse_matrix &out);

  /// Same as extract_block_pointer but keep column indices in output.
  void
  extract_block_pointer_keep_cols (const std::vector<int> &rows,
                                   const std::vector<int> &cols,
                                   p_sparse_matrix &out);

  /// Set all entries to 0 preserving storage structure.
  void
  reset ();

  /// Sparse matrix increment. Automatically allocates additional entries.
  template<class T>
  void 
  operator+= (T &adm);

  /// Compute matrix-vector product.
  friend std::vector<double>
  operator * (sparse_matrix& M, const std::vector<double>& x);

};

template<class T>
void
sparse_matrix::operator+= (T &adm)
{

  assert (this->rows () == adm.rows ());
  assert (this->cols () == adm.cols ());

  col_iterator jj;
  for (size_t ii = 0; ii < adm.size (); ++ii)
    if (adm[ii].size ())
      for (jj  = adm[ii].begin (); jj != adm[ii].end (); ++jj)
        (*this)[ii][jj->first] += adm.col_val (jj);
}

#endif
