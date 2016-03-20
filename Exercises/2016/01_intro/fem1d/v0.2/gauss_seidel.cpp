#include "gauss_seidel.h"
#include <cmath>

void
gauss_seidel
(const double A[][nnodes],
 const double f[],
 double uh[],
 const unsigned int maxit,
 const double tol)
{
  double uh_new = 0;
  double incrnorm = 0;
  for (unsigned int ii = 0; ii < maxit; ++ii)
    {
      incrnorm = 0;
      for (unsigned int jj = 0; jj < nnodes; ++jj)
        {
          double res = f[jj];

          for (unsigned int kk = 0; kk < nnodes; ++kk)
            if (kk != jj)
              res -= A[jj][kk] * uh[kk];
          
          uh_new = res / A[jj][jj];
          incrnorm = std::abs (uh_new - uh[jj]) > incrnorm ?
            std::abs (uh_new - uh[jj]) :
            incrnorm;
          uh[jj] = uh_new;
        }
      if (incrnorm < tol)
        break;
    }
}

