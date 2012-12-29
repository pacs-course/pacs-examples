
// @HEADER
// ***********************************************************************
// 
//                      Didasko Tutorial Package
//                 Copyright (2005) Sandia Corporation
// 
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
// 
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//  
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//  
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// Questions about Didasko? Contact Marzio Sala (marzio.sala _AT_ gmail.com)
// 
// ***********************************************************************
// @HEADER

// Basic definition of communicator.
// This code should be run with one process

#include "Didasko_ConfigDefs.h"
#if defined(HAVE_DIDASKO_EPETRA)

#include <iostream>
#include "Epetra_ConfigDefs.h"
#ifdef HAVE_MPI
#include "mpi.h"
#include "Epetra_MpiComm.h"
#else
#include "Epetra_SerialComm.h"
#endif
#include "Epetra_SerialDenseVector.h"
#include "Epetra_SerialDenseMatrix.h"
#include "Epetra_SerialDenseSolver.h"

int main(int argc, char *argv[])
{
  
#ifdef HAVE_MPI
  MPI_Init(&argc, &argv);
  Epetra_MpiComm Comm(MPI_COMM_WORLD);
#else
  Epetra_SerialComm Comm;
#endif

  // Total number of elements in vectors, can be any positive number
  int NumRows = 5;

  Epetra_SerialDenseVector x, b;
  x.Size( NumRows );
  b.Size( NumRows );

  // set the elements of the vector
  for( int i=0 ; i<NumRows ; ++i ) b[i] = 1.0, x[i]=0.0;
  
  Epetra_SerialDenseMatrix A, A2;
  A.Shape( NumRows, NumRows );
  A2.Shape( NumRows, NumRows ); // A2 is a copy of A

  // Hilbert matrix (ill-conditioned)
  for( int i=0 ; i<NumRows ; ++i )
    for( int j=0 ; j<NumRows ; ++j ) 
      A(i,j) = 1.0/(i+j+2);

  cout<< A;

  // set up the solver
  Epetra_SerialDenseSolver Problem;
  Problem.SetMatrix( A );
  Problem.SetVectors( x, b );

  A2 = A; 
  // we make a copy of A because Problem.Solve() will
  // overwrite A with its LU decomposition. Try with
  // cout << A after the following invocation

  b.Multiply('N','N',1.0, A2, x, 0.0);

  cout << "A * x = \n" << b;

  double rcond;
  Problem.ReciprocalConditionEstimate(rcond);
  cout << "The (estimated) condition number of A is " << 1/rcond << endl;

  Problem.SetMatrix( A2 );
  Problem.Invert();
  cout << "The inverse of A is\n";
  cout << A2;

#ifdef HAVE_MPI
  MPI_Finalize();
#endif

} /* main */

#else

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  puts("Please configure Didasko with:\n"
       "--enable-epetra");

  return 0;
}
#endif
