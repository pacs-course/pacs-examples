#ifndef HAVE_GAUSS_SEIDEL_H
#define HAVE_GAUSS_SEIDEL_H

template<unsigned int nnodes>
void
gauss_seidel (const std::array<std::array<double, nnodes>, nnodes> &A,
              const std::array<double, nnodes> &f,
              std::array<double, nnodes> &uh,
              const unsigned int maxit = 10000,
              const double tol = 1.0e-9)
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

#endif
