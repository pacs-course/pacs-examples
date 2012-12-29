#include <cstdlib>
#include <cassert>
#include <string>
#include <vector>

#include "Epetra_CrsMatrix.h"
#include "Epetra_MultiVector.h"
#include "Epetra_Time.h"
#include "Epetra_Vector.h"
#include "Epetra_Version.h"

#ifdef EPETRA_MPI
#  include "mpi.h"
#  include "Epetra_MpiComm.h"
#else
#  include "Epetra_SerialComm.h"
#endif

//
// Compute the eigenvalue of maximum magnitude of the given matrix A,
// using the power method.
// 
// Input arguments:
//
// A: The matrix to which to apply the power method.  It's not const
//   because this method sets its flop counter.
// niters: Number of iterations of the power method.
// tolerance: Iterate until the (absolute) residual is strictly less
//   than tolerance.
// verbose: Whether or not to print status output to stdout during
//   iterations.
//
// Output argument:
//
// lambda: The eigenvalue of maximum magnitude of the matrix A.
//
// Return value: An integer error code.  Zero if no error occured,
//   else nonzero.
// 
int 
powerMethod (double & lambda, 
             Epetra_CrsMatrix& A, 
             const int niters, 
             const double tolerance,
             const bool verbose);

int 
main (int argc, char *argv[])
{
  // These "using" statements make the code a bit more concise.
  using std::cout;
  using std::endl;

  int ierr = 0, i;

  // If Trilinos was built with MPI, initialize MPI, otherwise
  // initialize the serial "communicator" that stands in for MPI.
#ifdef EPETRA_MPI
  MPI_Init (&argc,&argv);
  Epetra_MpiComm Comm (MPI_COMM_WORLD);
#else
  Epetra_SerialComm Comm;
#endif

  const int MyPID = Comm.MyPID();
  const int NumProc = Comm.NumProc();
  // We only allow (MPI) Process 0 to write to stdout.
  const bool verbose = (MyPID == 0);
  const int NumGlobalElements = 100;

  if (verbose)
    cout << Epetra_Version() << endl << endl;

  // Asking the Epetra_Comm to print itself is a good test for whether
  // you are running in an MPI environment.  However, it will print
  // something on all MPI processes, so you should remove it for a
  // large-scale parallel run.
  cout << Comm << endl;

  if (NumGlobalElements < NumProc)
    {
      if (verbose)
        cout << "numGlobalBlocks = " << NumGlobalElements 
             << " cannot be < number of processors = " << NumProc << endl;
      std::exit (EXIT_FAILURE);
    }

  // Construct a Map that puts approximately the same number of rows
  // of the matrix A on each processor.
  Epetra_Map Map (NumGlobalElements, 0, Comm);

  // Get update list and number of local equations from newly created Map.
  int NumMyElements = Map.NumMyElements();

  std::vector<int> MyGlobalElements(NumMyElements);
  Map.MyGlobalElements(&MyGlobalElements[0]);

  // NumNz[i] is the number of nonzero elements in row i of the sparse
  // matrix on this MPI process.  Epetra_CrsMatrix uses this to figure
  // out how much space to allocate.
  std::vector<int> NumNz (NumMyElements);

  // We are building a tridiagonal matrix where each row contains the
  // nonzero elements (-1 2 -1).  Thus, we need 2 off-diagonal terms,
  // except for the first and last row of the matrix.
  for (int i = 0; i < NumMyElements; ++i)
    if (MyGlobalElements[i] == 0 || MyGlobalElements[i] == NumGlobalElements-1)
      NumNz[i] = 2; // First or last row
    else
      NumNz[i] = 3; // Not the (first or last row)

  // Create the Epetra_CrsMatrix.
  Epetra_CrsMatrix A (Copy, Map, &NumNz[0]);

  //
  // Add rows to the sparse matrix one at a time.
  //
  std::vector<double> Values(2);
  Values[0] = -1.0; Values[1] = -1.0;
  std::vector<int> Indices(2);
  const double two = 2.0;
  int NumEntries;

  for (int i = 0; i < NumMyElements; ++i)
    {
      if (MyGlobalElements[i] == 0)
        { // The first row of the matrix.
          Indices[0] = 1;
          NumEntries = 1;
        }
      else if (MyGlobalElements[i] == NumGlobalElements - 1)
        { // The last row of the matrix.
          Indices[0] = NumGlobalElements-2;
          NumEntries = 1;
        }
      else
        { // Any row of the matrix other than the first or last.
          Indices[0] = MyGlobalElements[i]-1;
          Indices[1] = MyGlobalElements[i]+1;
          NumEntries = 2;
        }
      ierr = A.InsertGlobalValues(MyGlobalElements[i], NumEntries, &Values[0], &Indices[0]);
      assert (ierr==0);
      // Insert the diagonal entry.
      ierr = A.InsertGlobalValues(MyGlobalElements[i], 1, &two, &MyGlobalElements[i]);
      assert(ierr==0);
    }

  // Finish up.  We can call FillComplete() with no arguments, because
  // the matrix is square.
  ierr = A.FillComplete ();
  assert (ierr==0);

  // Parameters for the power method.
  const int niters = NumGlobalElements*10;
  const double tolerance = 1.0e-2;

  //
  // Run the power method.  Keep track of the flop count and the total
  // elapsed time.
  //
  Epetra_Flops counter;
  A.SetFlopCounter(counter);
  Epetra_Time timer(Comm);
  double lambda = 0.0;
  ierr += powerMethod (lambda, A, niters, tolerance, verbose);
  double elapsedTime = timer.ElapsedTime ();
  double totalFlops =counter.Flops ();
  // Mflop/s: Million floating-point arithmetic operations per second.
  double Mflop_per_s = totalFlops / elapsedTime / 1000000.0;

  if (verbose) 
    cout << endl << endl << "Total Mflop/s for first solve = " 
         << Mflop_per_s << endl<< endl;

  // Increase the first (0,0) diagonal entry of the matrix.
  if (verbose) 
    cout << endl << "Increasing magnitude of first diagonal term, solving again"
         << endl << endl << endl;

  if (A.MyGlobalRow (0)) {
    int numvals = A.NumGlobalEntries (0);
    std::vector<double> Rowvals (numvals);
    std::vector<int> Rowinds (numvals);
    A.ExtractGlobalRowCopy (0, numvals, numvals, &Rowvals[0], &Rowinds[0]); // Get A(0,0)
    for (int i = 0; i < numvals; ++i) 
      if (Rowinds[i] == 0) 
        Rowvals[i] *= 10.0;

    A.ReplaceGlobalValues (0, numvals, &Rowvals[0], &Rowinds[0]);
  }

  //
  // Run the power method again.  Keep track of the flop count and the
  // total elapsed time.
  //
  lambda = 0.0;
  timer.ResetStartTime();
  counter.ResetFlops();
  ierr += powerMethod (lambda, A, niters, tolerance, verbose);
  elapsedTime = timer.ElapsedTime();
  totalFlops = counter.Flops();
  Mflop_per_s = totalFlops / elapsedTime / 1000000.0;

  if (verbose) 
    cout << endl << endl << "Total Mflop/s for second solve = " 
         << Mflop_per_s << endl << endl;

#ifdef EPETRA_MPI
  MPI_Finalize() ;
#endif

  return ierr;
}


int
powerMethod (double & lambda, 
             Epetra_CrsMatrix& A, 
             const int niters, 
             const double tolerance,
             const bool verbose)
{
  // In the power iteration, z = A*q.  Thus, q must be in the domain
  // of A, and z must be in the range of A.  The residual vector is of
  // course in the range of A.
  Epetra_Vector q (A.OperatorDomainMap ());
  Epetra_Vector z (A.OperatorRangeMap ());
  Epetra_Vector resid (A.OperatorRangeMap ());

  Epetra_Flops* counter = A.GetFlopCounter();
  if (counter != 0) {
    q.SetFlopCounter(A);
    z.SetFlopCounter(A);
    resid.SetFlopCounter(A);
  }

  // Initialize the starting vector z with random data.
  z.Random();

  double normz, residual;
  int ierr = 1;
  for (int iter = 0; iter < niters; ++iter)
    {
      z.Norm2 (&normz);        // normz  := ||z||_2
      q.Scale (1.0/normz, z);  // q      := z / normz
      A.Multiply(false, q, z); // z      := A * q
      q.Dot(z, &lambda);       // lambda := dot (q, z)

      // Compute the residual vector and display status output every
      // 100 iterations, or if we have reached the maximum number of
      // iterations.
      if (iter % 100 == 0 || iter + 1 == niters)
        {
          resid.Update (1.0, z, -lambda, q, 0.0); // resid := A*q - lambda*q
          resid.Norm2 (&residual);                // residual := ||resid||_2
          if (verbose) 
            cout << "Iter = " << iter << "  Lambda = " << lambda 
                 << "  Residual of A*q - lambda*q = " << residual << endl;
        } 
      if (residual < tolerance) { // We've converged!
        ierr = 0;
        break;
      }
    }
  return ierr;
}

