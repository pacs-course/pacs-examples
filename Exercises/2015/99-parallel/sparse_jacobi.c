#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

#define N 1001
#define h 0.001
#define maxit  1000000
#define tol  1e-12

int main(int argc, char ** argv )
{
  int *elements, rank, size, i, j, k, *I, *J, locdim, locnel, it, pippo;
  double  *D, *Dglob, *V, *u, *unew, *tmp, *b, *r, nrm;

    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    
    /* partition elements */
    elements = (int*) calloc ((size_t) size, sizeof (int));
    for (i = 0; i < size; i++)
      elements[i] = i * N / size;
    elements[size] = N-1;

    /* set up */
    locnel = (elements[rank+1] - elements[rank]);
    locdim = 4 * locnel;
    I = (int*) calloc ((size_t)locdim, sizeof (int));
    J = (int*) calloc ((size_t)locdim, sizeof (int));
    V = (double*) calloc ((size_t)locdim, sizeof (double));
    D = (double*) calloc ((size_t)N, sizeof (double));
    Dglob = (double*) calloc ((size_t)N, sizeof (double));
    u = (double*) calloc ((size_t)N, sizeof (double));
    unew = (double*) calloc ((size_t)N, sizeof (double));
    b =  (double*) calloc ((size_t)N, sizeof (double));
    r =  (double*) calloc ((size_t)N, sizeof (double));
    for (i = 0; i < N; i ++)
      b[i] = 1.0 * h;

    /* init elemental matrices */
#pragma omp parallel for default (none) shared (V, D, I, J, locnel, rank, elements) private (i,j,k)
    for (i = 0; i < locnel; i++)
      { 
        for (j = 0; j < 2; j++)
          {
            for (k = 0; k < 2; k++)
              {
                I[k+2*(j+2*i)]  = i + j + elements[rank];
                J[k+2*(j+2*i)]  = i + k + elements[rank];
                V[k+2*(j+2*i)]  = pow (-1.0, k+j)/h;
              }    
#pragma omp atomic
            D[I[j+2*(j+2*i)]] +=  1.0/h;                        
          }
      }

    /* communicate diagonal preconditioner */
    MPI_Allreduce (D, Dglob, N, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);


    /* jacobi iteration */
    for (it = 0; it < maxit; it++)
      {

        for (i = 0; i < N; i ++)
          r[i] = 0.0;

        /* compute - A * x */
#pragma omp parallel for default (none) shared (r, V, u, I, J, locnel) private (i,j,k)
        for (i = 0; i < locnel; i++)
          for (j = 0; j < 2; j++)
            for (k = 0; k < 2; k++)
#pragma omp atomic
              r[I[k+2*(j+2*i)]] -= V[k+2*(j+2*i)] * u[J[k+2*(j+2*i)]];

        /* communicate - A * x */
        MPI_Allreduce (r, unew, N, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        if (rank == 0)
          nrm = 0.0;

        for (i = 1; i < N-1; i++)
          {
            r[i] = (b[i] + unew[i]);
            unew[i] = r[i]/Dglob[i] + u[i];
            if (rank == 0)
              nrm += pow (r[i], 2);
          }
        unew[0] = 0.0;
        unew[N-1] = 0.0;

        /* communicate  residual norm squared */
        MPI_Bcast ( &nrm, 1, MPI_DOUBLE, 0,  MPI_COMM_WORLD );

        if (nrm < tol)
          break;
        
        /* swap u <-> unew */
        tmp = u; u = unew; unew = tmp;
        
      }

    if (rank == 0)
      {
        printf ("#after %d iterations, nrm^2 = %g\nu = [", it, nrm);
        for (i =0; i<N; i++)
          printf (" %.16f ", u[i]);
        printf("];\nx = [");
        for (i =0; i<N; i++)
          printf (" %.16f ", h * i);
        printf("];\n");
      }
    MPI_Barrier (MPI_COMM_WORLD);
    
    MPI_Finalize();
    return 0;
}
