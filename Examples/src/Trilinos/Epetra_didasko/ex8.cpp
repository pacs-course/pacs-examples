
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

// Epetra_Vectors in View mode; use of ResetView.

#include "Didasko_ConfigDefs.h"
#if defined(HAVE_DIDASKO_EPETRA)

#include "Epetra_ConfigDefs.h"
#ifdef HAVE_MPI
#include "mpi.h"
#include "Epetra_MpiComm.h"
#else
#include "Epetra_SerialComm.h"
#endif
#include "Epetra_Map.h"
#include "Epetra_Vector.h"

int main(int argc, char *argv[])
{
  
#ifdef HAVE_MPI
  MPI_Init(&argc, &argv);
  Epetra_MpiComm Comm(MPI_COMM_WORLD);
#else
  Epetra_SerialComm Comm;
#endif  

  // Total number of elements in the vector
  int NumLocalElements = 10;

  // Construct a Map with NumElements and index base of 0
  Epetra_Map Map(-1, NumLocalElements, 0, Comm);

  // here it is defined a double vector of size NumLocalElements
  // (that is, a size compatible with Map), and it is filled with
  // some values

  double* values; values = new double[NumLocalElements];
  for( int i=0 ;i<NumLocalElements ; i++ )
    values[i] = 1.0*i;
  
  // Create x as an Epetra_vector with the View mode, using `values'
  // as data
  
  Epetra_Vector x(View, Map, values);

  // now we can change x by modifying values...
  values[0] = 123;
  
  // this affects the object x
  cout << x;

  // now we can reset the view, and let it point to an other douoble
  // vector (having the same size)

  double* values2; values2 = new double[NumLocalElements];
  for( int i=0 ;i<NumLocalElements ; i++ )
    values2[i] = -1.0*i;

  x.ResetView( values2 );

  cout << x;
  
  delete[] values;
  delete[] values2;

#ifdef HAVE_MPI
  MPI_Finalize();
#endif
  
  return(0);

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
