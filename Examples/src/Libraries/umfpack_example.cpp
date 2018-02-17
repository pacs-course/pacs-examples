#include "umfpack_example.hpp"

Vector solve_with_umfpack(int n, Index const & Ap,
                          Index const & Ai,
                          Vector const & Ax,
                          Vector const & b)
{
  double *null = (double *) NULL ;
  void *Symbolic, *Numeric ;
  umfpack_di_symbolic (n, n, Ap.data(), Ai.data(), Ax.data(),
                       &Symbolic, null, null) ;
  umfpack_di_numeric (Ap.data(), Ai.data(), Ax.data(),
                      Symbolic, &Numeric, null, null) ;
  umfpack_di_free_symbolic (&Symbolic) ;
  double* xp=new double[n];
  umfpack_di_solve (UMFPACK_A, Ap.data(), Ai.data(), Ax.data(), xp, b.data(), Numeric, null, null) ;
  umfpack_di_free_numeric (&Numeric) ;
  Vector x(xp,xp+n);
  delete[] xp;
  return x;
}
