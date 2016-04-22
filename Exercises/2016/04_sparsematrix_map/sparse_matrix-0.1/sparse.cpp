#include "sparse.h"


sparse_matrix::sparse_matrix (unsigned int size)
{
  this->resize (size);
  nrows = size;
  ncols = size;
}

sparse_matrix::sparse_matrix (unsigned int m, unsigned int n)
{
  this->resize (m);
  nrows = m;
  ncols = n;
}

double &
sparse_matrix::operator() (unsigned int irow, unsigned int jcol)
{
  assert (irow < nrows);
  assert (jcol < ncols);
  return (*this)[irow][jcol];    
};

const double &
sparse_matrix::operator()  (unsigned int irow, unsigned int jcol) const
{
  assert (irow < nrows);
  assert (jcol < ncols);
  auto retval = (*this)[irow].find (jcol);
  if (retval != (*this)[irow].end ())
    return retval->second;
  else
    return zero;
};

sparse_matrix
sparse_matrix::transpose () const
{
  sparse_matrix retval (this->get_cols (),
                        this->get_rows ());
  for (unsigned int ii = 0; ii < this->nrows; ++ii)
    for (auto jj = (*this)[ii].begin ();
         jj != (*this)[ii].end (); ++jj)
      retval(jj->first, ii) = jj->second;
  return retval;
};

void
sparse_matrix::gauss_seidel
(const matrix &f,
 matrix &uh,
 const unsigned int maxit,
 const double tol)
{

  assert (uh.get_rows () == f.get_rows ());
  assert (uh.get_rows () == this->get_rows ());
  assert (uh.get_cols () == 1);
  
  double uh_new = 0;
  double incrnorm = 0;  
  
  for (unsigned int ii = 0; ii < maxit; ++ii)
    {
      incrnorm = 0;
      for (unsigned int jj = 0; jj < get_rows (); ++jj)
        {
          double res = f(jj, 0);

          for (auto kk = (*this)[jj].begin ();
               kk != (*this)[jj].end (); ++kk)
            if (kk->first != jj)
              res -= kk->second * uh(kk->first, 0);
          
          uh_new = res / (*this)(jj,jj);

          incrnorm = std::abs (uh_new - uh(jj, 0)) > incrnorm ?
            std::abs (uh_new - uh(jj, 0)) :
            incrnorm;

          uh(jj, 0) = uh_new;
        }
        
      if (incrnorm < tol)
        break;
    }
    
};



sparse_matrix
operator* (const sparse_matrix& A, const sparse_matrix& B)
{
  assert (A.get_cols () == B.get_rows ());
  sparse_matrix retval (A.get_rows (),
                        B.get_cols ());

  for (unsigned int ii = 0; ii < A.get_rows (); ++ii)
    for (unsigned int kk = 0; kk < B.get_cols (); ++kk)
      {
        double tmp = 0.0;
        for (auto jj = A[ii].begin ();
             jj != A[ii].end (); ++jj)
          tmp += jj->second * B(jj->first, kk);

        if (tmp != 0.0)
          retval(ii, kk) = tmp;
      }
  return retval;
};

matrix
operator* (const sparse_matrix& A, const matrix& B)
{
  assert (A.get_cols () == B.get_rows ());
  matrix retval (A.get_rows (), B.get_cols ());

  for (unsigned int ii = 0; ii < A.get_rows (); ++ii)
    for (unsigned int kk = 0; kk < B.get_cols (); ++kk)
      {
        retval(ii, kk) = 0;
        for (auto jj = A[ii].begin ();
             jj != A[ii].end (); ++jj)
          retval(ii, kk) += jj->second * B(jj->first, kk);
      }
  return retval;
  
};

matrix
operator* (const matrix& A, const sparse_matrix& B)
{

  assert (A.get_cols () == B.get_rows ());  
  auto At = A.transpose ();
  auto Bt = B.transpose ();
  matrix tmp = (Bt * At);
  matrix retval = tmp.transpose ();
  return retval;
  
};

#endif
