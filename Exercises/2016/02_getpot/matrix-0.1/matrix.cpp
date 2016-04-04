#include "matrix.h"
#include <cassert>
#include <algorithm>
#include <ctime>

#define dgesv dgesv_
#define DGESV dgesv_

#define dgemm dgemm_
#define DGEMM dgemm_

extern "C"
{
  void 
  dgesv (int *N, int *NRHS, double *A, int *LDA,
         int *IPIV, double *B, int *LDB, int *INFO);

  void
  dgemm (char *TRANSA, char *TRANSB, int *M, int *N, int *K,
         double *ALPHA, const double *A, int *LDA, const double *B, int *LDB,
         double *BETA, double *C, int *LDC);
}

matrix::matrix (const matrix& m) :
  rows (m.get_rows ()), cols (m.get_cols ())
{
  std::copy (m.get_data (),
             m.get_data () + m.get_cols () *
             m.get_rows (), data.begin ());
}


matrix
matrix::transpose () const
{
  matrix retval (get_cols (), get_rows ());
  unsigned int i, j;
  for (i = 0; i < rows; ++i)
    for (j = 0; j < cols; ++j)
      retval (i, j) = const_index (j, i);
  return (retval);
}

#ifdef USE_DGEMM
matrix
operator* (const matrix& A, const matrix& B)
{
  int M = A.get_rows ();
  int N = B.get_cols ();
  int K = A.get_cols ();
  char ntr = 'n';
  double one = 1.0;
  double zero = 0.0;
  assert (K == B.get_rows ());
  matrix retval (M, N);
  const double *Adata = A.get_data ();
  const double *Bdata = B.get_data ();
  double *Cdata = retval.get_data ();
  dgemm (&ntr, &ntr, &M, &N, &K,
         &one, Adata, &M, Bdata,
         &K, &zero, Cdata, &M);
  return (retval);
}
#elif defined (MAKE_TMP_TRANSP)
matrix
operator* (const matrix& A, const matrix& B)
{
  assert (A.get_cols () == B.get_rows ());
  matrix retval (A.get_rows (), B.get_cols ());
  matrix tmp = A.transpose ();
  for (unsigned int i = 0; i < retval.get_rows (); ++i)
    for (unsigned int j = 0; j < retval.get_cols (); ++j)
      for (unsigned int k = 0; k < A.get_cols (); ++k)
        retval(i,j) += tmp(k,i) * B(k,j);
  return (retval);
}
#else
matrix
operator* (const matrix& A, const matrix& B)
{
  assert (A.get_cols () == B.get_rows ());
  matrix retval (A.get_rows (), B.get_cols ());
  for (unsigned int i = 0; i < retval.get_rows (); ++i)
    for (unsigned int j = 0; j < retval.get_cols (); ++j)
      for (unsigned int k = 0; k < A.get_cols (); ++k)
        retval(i,j) += A(i,k) * B(k,j);
  return (retval);
}
#endif

void
matrix::solve (matrix &rhs)
{
  int N = get_rows ();
  int IPIV[N];
  int NRHS = rhs.get_cols ();
  int LDB = rhs.get_rows ();
  int INFO = 0;
  dgesv (&N, &NRHS, get_data (), &N,
         IPIV, rhs.get_data (), &LDB, &INFO);
};


void
matrix::gauss_seidel
(const matrix &f,
 matrix &uh,
 const unsigned int maxit,
 const double tol)
{

  assert (uh.get_rows () == f.get_rows ());
  assert (uh.get_rows () == get_rows ());
  assert (uh.get_cols () == 1);
  
  double uh_new = 0;
  double incrnorm = 0;  
  
  for (unsigned int ii = 0; ii < maxit; ++ii)
    {
      incrnorm = 0;
      for (unsigned int jj = 0; jj < get_rows (); ++jj)
        {
          double res = f(jj, 0);

          for (unsigned int kk = 0; kk < get_cols (); ++kk)
            if (kk != jj)
              res -= const_index(jj,kk) * uh(kk, 0);
          
          uh_new = res / const_index(jj,jj);
          incrnorm = std::abs (uh_new - uh(jj, 0)) > incrnorm ?
            std::abs (uh_new - uh(jj, 0)) :
            incrnorm;
          uh(jj, 0) = uh_new;
        }
      if (incrnorm < tol)
        break;
    }
};
