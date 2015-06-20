#include <stdlib.h>                        // System includes
#include <iostream>                      // 
using std::endl;
using std::cin;
using std::cout;
using std::cerr;


#include "compcol_double.h"                // Compressed column matrix header
#include "iohb_double.h"                   // Harwell-Boeing matrix I/O header
#include "mvblasd.h"                       // MV_Vector level 1 BLAS
#include "diagpre_double.h"                // Diagonal preconditioner

#include "bicg.h"                          // IML++ BiCG template

int
main(int argc, char * argv[])
{
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " HBfile " << endl;
    exit(-1);
  }

  double tol = 1.e-6;                      // Convergence tolerance
  int result, maxit = 150;                 // Maximum iterations

  CompCol_Mat_double A;                    // Create a matrix
  readHB_mat(argv[1], &A);                 // Read matrix data
  VECTOR_double b, x(A.dim(1), 0.0);       // Create rhs, solution vectors
  readHB_rhs(argv[1], &b);                 // Read rhs data

  DiagPreconditioner_double D(A);          // Create diagonal preconditioner

  result = BiCG(A, x, b, D, maxit, tol);   // Solve system

  cout << "BiCG flag = " << result << endl;
  cout << "iterations performed: " << maxit << endl;
  cout << "tolerance achieved  : " << tol << endl;

  return result;
}
