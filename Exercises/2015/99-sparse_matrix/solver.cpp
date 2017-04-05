/*
 * solver.cpp
 *
 *  Created on: Jan 14, 2009
 *      Author: Paolo G. Ferrandi
 */

// The simplest code of ever!

#include <iostream>
#include <stdlib.h>

// Hint: write a proper Makefile and remove all absolute path.
#include "cg.h" // Conjugate gradient
#include "ilupre_double.h" // iLU precondtioner
#include "compcol_double.h" //Compressed column sparse matrix
#include "iohb_double.h" // Read a Harwell-Boeing file into a compressed Column matrix


using namespace std;

int main(){

    // Hint: read arguments from command line (with Getpot)
    double tol=1e-6;
    int iter=1e3; // max. iterations
    int noconv;

    CompCol_Mat_double A;
    readHB_mat("nos4.rsa", &A);
    VECTOR_double sol(A.dim(1), 1.0);
    VECTOR_double x(A.dim(1), 1e10);
    VECTOR_double b=A*sol;

    CompCol_ILUPreconditioner_double P(A);
    noconv = CG(A, x,b,P,iter,tol);

    if (noconv==1) cout << "No convergence" << endl;
        else
        {
            cout << "Iteration performed: " << iter << endl;
            cout << "Tolerance achieved: " << tol << endl;
            cout << x;
        }

    return 0;
}

