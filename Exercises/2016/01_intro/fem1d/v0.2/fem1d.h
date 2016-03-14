#ifndef HAVE_FEM1D_H
#define HAVE_FEM1D_H 

#include <array>
#include <algorithm>
#include <cmath>

class mesh
{

public:

  const unsigned int nnodes;
  const double L;
  const unsigned int nels;
  const double h;

  double *nodes;
  unsigned int (*elements)[2];

  mesh (const double a, const double b, const unsigned int nnodes_)
    : L (b - a), nnodes (nnodes_), nels (nnodes - 1), h (L / double (nels))
  {
    
    nodes = new double [nnodes];
    for (unsigned int ii = 0; ii < nnodes; ++ii)
      nodes[ii] = static_cast<double>(ii) * h;

    elements = new unsigned int [2][nels];
    for (unsigned int ii = 0; ii < nels; ++ii)
      {
        elements[ii][0] = ii;
        elements[ii][1] = ii+1;
      }
  }

  ~mesh ()
  {
    delete [] nodes;
    delete [] elements;
  }
  
};
  
void
gauss_seidel
(const double[][] &A,
 const double[] &f,
 double[] &uh,
 const unsigned int nnodes,
 const unsigned int maxit = 500,
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
