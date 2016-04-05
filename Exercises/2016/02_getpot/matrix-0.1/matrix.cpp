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
  dgesv (const int *N, const int *NRHS, const double *A,
         const int *LDA, int *IPIV, double *B, const int *LDB,
         int *INFO);

  void
  dgemm (const char *TRANSA, const char *TRANSB, const int *M,
         const int *N, const int *K, const double *ALPHA,
         const double *A, const int *LDA, const double *B,
         const int *LDB, const double *BETA, double *C,
         const int *LDC);
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
  assert (K == B.get_rows ());
  
  char ntr = 'n';
  double one = 1.0;
  double zero = 0.0;

  matrix retval (M, N);
  
  dgemm (&ntr, &ntr, &M, &N, &K,
         &one, A.get_data (), &M,
         B.get_data (), &K, &zero,
         retval.get_data (), &M);

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
  matrix tmp ((*this));
  dgesv (&N, &NRHS, tmp.get_data (), &N,
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
