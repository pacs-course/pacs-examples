#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <mpi.h>
#include "GetPot"

// Some global variables
const int NDef       = 1001;
const int maxitDef   = 100;
const int outputDef  = 0;
const double tolDef  = 1e-12;

/*
 * Options are given in the file problem.dat in the form
 * N= n nodes
 * maxit= max n. iteration
 * tol= tolerance
 * Default values are given via global variables
 */

/*!
 *  helper function to return the index of local nodes (k,j) of element i
 */
inline int index(int k, int i, int j)
{
  return k+2*(j+2*i);
}

int main(int argc, char ** argv )
{
  int *elements, rank, size, *I, *J, locdim, locnel;
  double  *D, *Dglob, *V, *u, *unew, *b, *r, nrm;
  // Initialize MPI
  MPI_Init( &argc, &argv );
  // Get basic MPI information
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  // Read options from file problem .dat
  GetPot cl("problem.dat");
  int N=cl("N",NDef);
  int maxit=cl("maxit",maxitDef);
  double tol=cl("tol", tolDef);
  int output = cl ("output", outputDef);
  if (rank==0)
    std::cout<< " N. nodes="<<N<<" Max. iter="
             <<maxit<<" Tolerance= "<<tol<<std::endl;
  /* partition elements */
  // Domain is (0,1)
  double h = 1./(N-1);
  elements = new int[size+1];

  for (int i = 0; i < size; ++i)
    elements[i] = i * N / size;
  elements[size] = N-1;

  /* set up  elements local to partition */
  locnel = (elements[rank+1] - elements[rank]);
  locdim = 4 * locnel;
  // Allocate data

  I = new int[locdim];
  J = new int[locdim];
  V = new double[locdim];
  D = new double[N];

  Dglob = new double[N];
  u     = new double[N];
  unew  = new double[N];
  b     = new double[N];
  r     = new double[N];

  for (int i = 0; i < N; ++i) b[i] = 1.0 * h;

  /* initialize elemental matrices */
  ///#pragma omp parallel for default (none) shared (V, D, I, J, locnel, rank, elements) private (i,j,k)
#pragma omp parallel for default (none) shared (V, D, I, J, locnel, rank, elements, h)
  for (int i = 0; i < locnel; ++i)
    {
      for (int j = 0; j < 2; ++j)
        {
          for (int k = 0; k < 2; ++k)
            {
              int pos=index(k,i,j);
              I[pos]  = i + j + elements[rank];
              J[pos]  = i + k + elements[rank];
              V[pos]  = std::pow(-1.0, k+j)/h;
            }
#pragma omp atomic
          D[ I[index(j,i,j)] ] +=  1.0/h;
        }
    }

  /* communicate diagonal preconditioner */
  MPI_Allreduce (D, Dglob, N, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);


  /* jacobi iteration */
  int it; // I need it later on!
  for (it = 0; it < maxit; ++it)
    {

      for (int i = 0; i < N; i ++)
        r[i] = 0.0;

      /* compute - A * x */
#pragma omp parallel for default (none) shared (r, V, u, I, J, locnel)
      //#pragma omp parallel for default (none) shared (r, V, u, I, J, locnel) private (i,j,k)
      for (int i = 0; i < locnel; i++)
        for (int j = 0; j < 2; j++)
          for (int k = 0; k < 2; k++)
#pragma omp atomic
            r[I[index(k,i,j)]] -= V[index(k,i,j)] * u[J[index(k,i,j)]];

      /* communicate - A * x */
      MPI_Allreduce (r, unew, N, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

      if (rank == 0) nrm = 0.0;

      for (int i = 1; i < N-1; i++)
        {
          r[i] = (b[i] + unew[i]);
          unew[i] = r[i]/Dglob[i] + u[i];
          if (rank == 0) nrm += pow (r[i], 2);
        }
      unew[0] = 0.0;
      unew[N-1] = 0.0;

      /* communicate  residual norm squared */
      MPI_Bcast ( &nrm, 1, MPI_DOUBLE, 0,  MPI_COMM_WORLD );

      if (nrm < tol)  break;

      /* swap u <-> unew */
      std::swap(u,unew);
    }
  if (rank == 0)
    {

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
    }
  MPI_Barrier (MPI_COMM_WORLD);

  // Be a nice guy and clean up memory
  delete [] elements;
  delete [] I;
  delete [] J;
  delete [] V;
  delete [] D;
  delete [] Dglob;
  delete [] u;
  delete [] unew;
  delete [] b;
  delete [] r;

  // Close MPI
  MPI_Finalize();
  return 0;
}
