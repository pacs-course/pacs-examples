#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include "GetPot"

// Some global variables
const int NDef       = 1001;
const int maxitDef   = 100;
const int outputDef  = 0;
const double tolDef  = 1e-12;
/*
 * Options are given in the file problem.dat in the form
 * N = n nodes
 * maxit= max n. iteration
 * tol= tolerance
 * Default values are given via global variables
 */

/*!
 *  helper function to return the index of local nodes (k,j) of element i
 */
inline int index (int k, int i, int j) { return k+2*(j+2*i); }

int main (int argc, char ** argv )
{
  int  *I, *J, locdim;
  double *D, *V, *u, *unew, *b, *r, nrm;

  // Read options from file problem .dat
  GetPot cl ("problem.dat");

  int N      = cl ("N", NDef);
  int maxit  = cl ("maxit", maxitDef);
  int output = cl ("output", outputDef);
  double tol = cl ("tol", tolDef);

  std::cout << " N. nodes = "<< N << " Max. iter="
            << maxit << " Tolerance = " 
            << tol << std::endl;


  // Compute grid properties, Domain is (0,1)
  double h = 1. / (N-1);
  locdim = 4 * (N-1);

  // Allocate data
  I = new int[locdim];
  J = new int[locdim];
  V = new double[locdim];
  D = new double[N];

  u     = new double[N];
  unew  = new double[N];
  b     = new double[N];
  r     = new double[N];

  for (int i = 0; i < N; ++i) b[i] = 1.0 * h;
  for (int i = 1; i < N-1; ++i) u[i] = 1.0;

  /* initialize elemental matrices */
  ///#pragma omp parallel for default (none) shared (V, D, I, J, h, N) private (i,j,k)
#pragma omp parallel for default (none) shared (V, D, I, J, h, N)
  for (int i = 0; i < N-1; ++i)
    {

      for (int j = 0; j < 2; ++j)
        {
          for (int k = 0; k < 2; ++k)
            {

              int pos = index (k, i, j);
              I[pos]  = i + j;
              J[pos]  = i + k;
              V[pos]  = std::pow (-1.0, k+j) / h;

            }

#pragma omp atomic
          D[I[index (j, i, j)] ] +=  1.0 / h;
        }
    }


  /* jacobi iteration */
  int it; // I need it later on!
  
  for (it = 0; it < maxit; ++it)
    {

      for (int i = 0; i < N; i ++)
        r[i] = 0.0;

      /* compute - A * x */
#pragma omp parallel for default (none) shared (r, V, u, I, J, N, locdim)
      for (int i = 0; i < N-1; i++)
        for (int j = 0; j < 2; j++)
          for (int k = 0; k < 2; k++)
	    {
	      int idx = index (k, i, j);
#pragma omp atomic
	      r[I[idx]] -= V[idx] * u[J[idx]];
	    }
      nrm = 0.0;

      /* compute r = b - A * x */
#pragma omp parallel for default (none) shared (r, D, u, unew, b, N) \
  reduction (+:nrm)
      for (int i = 1; i < N-1; i++)
        {
          r[i] += b[i];
          unew[i] = r[i] / D[i] + u[i];

          nrm += pow (r[i], 2);
        }

      unew[0] = 0.0;
      unew[N-1] = 0.0;

      if (nrm < tol)  break;

      /* swap u <-> unew */
      std::swap(u,unew);
     
    }

  std::cout<<"#after "<< it<<" iterations, nrm = "<<
    std::sqrt(nrm)<<std::endl;

  // Write results in a file 
  if (output > 0)
    {
      std::ofstream fout;
      switch (output)
        {
        case 1:
          // Writes solution and grid in ascii format
          fout.open ("result.m");
          fout << "u = [";
          for (int i = 0; i < N; i++) { fout << std::setprecision(16)  << u[i] << "; "; }
          fout << "];" << std::endl;
          
          fout << "x = [";
          for (int i = 0; i < N; i++) { fout << std::setprecision(16) << h*static_cast<double>(i) << "; "; }
          fout << "];" << std::endl;
          
          fout << "I = [";
          for (int i = 0; i < locdim; i++) { fout << I[i] << "; "; }
          fout << "];" << std::endl;
          
          fout << "J = [";
          for (int i = 0; i < locdim; i++) { fout << J[i] << "; "; }
          fout << "];" << std::endl;
          
          fout << "V = [";
          for (int i = 0; i < locdim; i++) { fout << V[i] << "; "; }
          fout << "];" << std::endl;
          
          fout << "x = [";
          for (int i = 0; i < N; i++) { fout << std::setprecision(16) 
                                             << h*static_cast<double>(i) << "; "; }
          fout << "];" << std::endl;

          break;
          
        case 2:          
          // Write results in a binary file
          fout.open ("result.dat",std::ios::binary);

          // Writes solution and grid
          fout.write((char*)u,N*sizeof(double));
          for (int i =0; i<N; i++)
            u[i] = h*i;

          fout.write((char*)u,N*sizeof(double));
        }
      
      fout.close();
    }

  // Be a nice guy and clean up memory
  delete [] I;
  delete [] J;
  delete [] V;
  delete [] D;
  delete [] u;
  delete [] unew;
  delete [] b;
  delete [] r;
  
  return 0;
}
