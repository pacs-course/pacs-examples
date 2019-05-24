/*
  Copyright (C) 2011 Carlo de Falco, http://mox.polimi.it/~carlo

  Copyright (C) 2006-2009 Evgenii Rudnyi, http://MatrixProgramming.com
  http://Evgenii.Rudnyi.Ru/

  The program reads the matrix and RHS in the Matrix Market format then run MUMPS.

  This software is a copyrighted work licensed under the terms, described
  in the file "FREE_LICENSE". 
*/

#define F77_COMM_WORLD -987654
#define JOB_INIT -1 
#define JOB_ANALYSIS 1
#define JOB_FACTORIZATION 2
#define JOB_BWSUB 3
#define JOB_SOLVE 6
#define JOB_END  -2


#include <fstream>
#include <iomanip>
#include <unistd.h>

#include "matrices.h"
#include "auxiliary.h"

#include "dmumps_c.h"

extern "C"
{ void open_stream_ (int* num, char name[]); }


#include <mpi.h>

int main (int argc, char **argv)
{	

  // 0) Init MPI
  MPI_Init (&argc, &argv);
  int rank, size;
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Comm_size (MPI_COMM_WORLD, &size);

  char stream_name[256]; int stream_num = 11 + rank;
  sprintf (stream_name, "mumps_log.%d.txt", rank);
  open_stream_ (&stream_num, stream_name);
  
  for (int ii=0; ii < size; ++ii)
    {
      if (ii == rank)
	{
	  char name[255];
	  gethostname (name, 255);
	  cout << "process " << rank << " running on " << name << endl;
	}
      MPI_Barrier (MPI_COMM_WORLD);
    }

  // 1) Read matrix on the host
  SparseMatrix mat;                             
  int IsSymmetric = 0;
  if (rank == 0)
    {
      Timing t0;
      ifstream in (argv[1]);
      if (!in)
        {
          cout << "cannot open file " << argv[1] << endl;
          MPI_Finalize ();
          return 2;
        }

      cout << "reading matrix from file: " << argv[1] << endl;
      mat.read (in);
      
      // if you know that you matrix is positive definite, IsSymmetric should be 1, then it is faster
      IsSymmetric = mat.IsSymmetric ? 2 : 0;
      t0.write ("Reading matrix");
    }

  MPI_Bcast (&IsSymmetric, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // 1a) setting up MUMPS parameters and initialize it
  DMUMPS_STRUC_C id;
  id.job          = JOB_INIT; 
  id.par          = 1;  
  id.sym          = IsSymmetric;
  id.comm_fortran = F77_COMM_WORLD;
  dmumps_c(&id);

  //streams    
  id.icntl[0] =   stream_num; 
  id.icntl[1] =   stream_num; 
  id.icntl[2] =   stream_num; 
  id.icntl[3] =   4;

 //centralized RHS and solution
  id.icntl[21] = 0;

  //ordering AMD (0), or metis (5), or pord (4), or AMF (2), or QAMD (6), or let MUMPS decide (7)	
  id.icntl[6] = 6;

  // 2) converting matrix to the MUMPS representation 
  
  vector<int> irn;
  vector<int> jcn;
  vector<double> a;

  if (rank == 0)
    {  
      Timing t0;
      id.n = mat.NRows ();
      id.nz = mat.nnz;
      irn.resize (id.nz);
      jcn.resize (id.nz);
      a.resize (id.nz);
      int k = 0;
      for (int i = 0; i < id.n; ++i)
        for (map<int, double>::iterator j = mat[i].begin (); j != mat[i].end (); ++j)
          {
            irn[k] = i + 1;
            jcn[k] = (*j).first + 1;
            a[k] = (*j).second;
            ++k;
          }
      id.irn = &*irn.begin (); 
      id.jcn = &*jcn.begin ();
      id.a   = &*a.begin ();
      t0.write ("converting matrix to the MUMPS representation");
    } 
  mat.clearMemory();
    
  
  // 3) Read RHS and set up MUMPS
  Matrix rhs;
  if (rank == 0)
    {
      Timing t0;
      ifstream in2(argv[2]);
      if (!in2)
        {
          cout << "cannot open file " << argv[2] << endl;
          return 4;
        }

      cout << "reading rhs from file: " << argv[2] << endl;
      rhs.read (in2);

      id.rhs = &*rhs.begin ();
      id.nrhs = rhs.NCols ();
      id.lrhs = id.n;
      t0.write ("Reading RHS and set up MUMPS");
    }

  // 4) Solve system
  Timing t1;
  id.job = JOB_SOLVE;
  dmumps_c (&id);
  if (rank == 0) t1.write ("Solving the linear system");

  // 5) write the solution out
  //solution is in rhs (it is overwritten)

  if (rank == 0)
    {
      Timing t0;
      cout << "Saving solution in " << argv[2] << ".solution" << endl;
      ofstream outf ((string (argv[2]) + ".solution").c_str ());
      rhs.write(outf);
      t0.write ("writing solution to file");
    }

  // 6) clean up
  Timing t2;
  id.job = JOB_END; 
  dmumps_c (&id);
  if (rank == 0) t2.write ("freeing memorly");

  MPI_Finalize ();
  return 0;	
}

