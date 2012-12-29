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

// Basic operations on vectors. The code defines two vectors and
// compute this dot product.

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
  int NumElements = 5;

  // Construct a Map with NumElements and index base of 0
  Epetra_Map Map(NumElements, 0, Comm);

  // Create x and b vectors
  Epetra_Vector x(Map);
  Epetra_Vector b(Map);

  // set all the elements to a scalar value
  x.Random( );

  // random numbers
  b.Random();

  // minimum, maximum and average of vector
  double minval, maxval, aveval;
  x.MinValue( &minval );
  x.MaxValue( &maxval );
  x.MeanValue( &aveval );
  cout << " min(x) = " << minval << endl;
  cout << " max(x) = " << maxval << endl;
  cout << " ave(x) = " << aveval << endl;

  // dot product
  double xdotb;
  x.Dot( b, &xdotb );
  cout << "x dot b = " << xdotb << endl;

  // total number of vectors, local and global dimension
  int NVecs, MySize, GloSize;
  NVecs = x.NumVectors(); 
  MySize = x.MyLength();
  GloSize = x.GlobalLength();
  cout << "Number of vectors = " << NVecs << endl;
  cout << "Local Size = " << MySize << endl;
  cout << "Global Size = " << GloSize << endl;

#ifdef HAVE_MPI
  MPI_Finalize();
#endif

  return( EXIT_SUCCESS );

}

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

