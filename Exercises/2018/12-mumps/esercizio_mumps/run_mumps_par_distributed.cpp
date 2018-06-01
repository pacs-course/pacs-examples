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
#include <cstdio>

#include "matrices.h"
#include "auxiliary.h"

extern "C"
{
#include "dmumps_c.h"
}

extern "C"
{
  void open_stream_ (int* num, char name[]);
}

#include <mpi.h>

int main (int argc, char **argv)
{	

  // 0) Init MPI
  MPI_Init (&argc, &argv);
  int rank, size;
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Comm_size (MPI_COMM_WORLD, &size);

  ostringstream outfilename;
  outfilename << "outfile."  << rank << ".txt";
  ofstream outfile (outfilename.str ().c_str ());

  char name[255];
  gethostname (name, 255);
  outfile << "process " << rank << " running on " << name << endl;

  // 1) Read matrix on the host
  SparseMatrix mat;                             
  int IsSymmetric = 0;
  ostringstream matfile;
  matfile << argv[1] << "."  << rank;
 
  Timing tread;
  ifstream in (matfile.str ().c_str ());
  if (!in)
    {
      cerr << "cannot open file " << matfile.str ().c_str () << endl;
      MPI_Finalize ();      return 2;
    }
  
  outfile << "process " << rank << " reading matrix from file: " <<  matfile.str ().c_str () << endl;
  mat.read (in);
  
  // if you know that you matrix is positive definite, IsSymmetric should be 1, then it is faster
  IsSymmetric = mat.IsSymmetric ? 2 : 0;
  
  
  MPI_Barrier (MPI_COMM_WORLD);
  if (rank == 0) tread.write ("Reading matrix");

  MPI_Bcast (&IsSymmetric, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // 1a) setting up MUMPS parameters and initialize it
  DMUMPS_STRUC_C id;
  id.job          = JOB_INIT; 
  id.par          = 1;  
  //id.sym          = IsSymmetric;
  id.sym          = 1; // SPD matrix
  id.cntl[0]      = 0.0; // setting id.cntl[0] = 0.0 will disble pivoting
  id.comm_fortran = F77_COMM_WORLD;
  dmumps_c(&id);

  char stream_name[256]; int stream_num = 11 + rank;
  sprintf (stream_name, "mumps_log.%d.txt", rank);
  open_stream_ (&stream_num, stream_name);

  //streams    
  id.icntl[0] =  stream_num; 
  id.icntl[1] =  stream_num; 
  id.icntl[2] =  stream_num; 
  id.icntl[3] =  3; // Lots of output

  //distributed assembled matrix
  id.icntl[4]  = 0;
  id.icntl[17] = 3;

  //centralized RHS and solution
  id.icntl[19] = 0;
  id.icntl[20] = 0;

  //iterative refinement
  id.icntl[9] = 10;

  //error analysis
  //id.icntl[10] = 1;
  
  //ordering AMD (0), or metis (5), or pord (4), or AMF (2), or QAMD (6), or let MUMPS decide (7)	
  id.icntl[6] = 7;

  //parallel ordering 0 = let MUMPS decide, 1 = NO, 2 = YES
  id.icntl[27] = 0;
  
  //parallel ordering 0 = let MUMPS decide, 1 = PT-SCOTCH, 2 = PARMETIS
  id.icntl[28] = 0;
  
  //file where to dump matrix
  //sprintf (id.write_problem, "matdump");

  // 2) converting matrix to the MUMPS representation 
  
  Timing t0;

  vector<int> irn;
  vector<int> jcn;
  vector<double> a;

  id.n = mat.NRows ();
  id.nz_loc = mat.nnz;

  irn.resize (id.nz_loc);
  jcn.resize (id.nz_loc);
  a.resize (id.nz_loc);
  int k = 0;
  for (int i = 0; i < id.n; ++i)
    for (map<int, double>::iterator j = mat[i].begin (); j != mat[i].end (); ++j)
      {
	irn[k] = i + 1;
	jcn[k] = (*j).first + 1;
	a[k] = (*j).second;
	++k;
      }
  id.irn_loc = &*irn.begin (); 
  id.jcn_loc = &*jcn.begin ();
  id.a_loc   = &*a.begin ();

  mat.clearMemory();
  MPI_Barrier (MPI_COMM_WORLD);
  if (rank == 0) t0.write ("converting matrix to the MUMPS representation");

    
  // 3) Read RHS and set up MUMPS

  Matrix rhs;
  if (rank == 0)
    {
      Timing t0;
      ifstream in2(argv[2]);
      if (!in2)
        {
          cerr << "cannot open file " << argv[2] << endl;
          MPI_Finalize ();      return 4;
        }

      outfile << "reading rhs from file: " << argv[2] << endl;
      rhs.read (in2);

      id.rhs = &*rhs.begin ();
      id.nrhs = rhs.NCols ();
      id.lrhs = id.n;
      t0.write ("Reading RHS and set up MUMPS");
    }
  MPI_Barrier (MPI_COMM_WORLD);

  // 4) Solve system once
 
  Timing ta;
  id.job = 1;  MPI_Barrier (MPI_COMM_WORLD);  dmumps_c (&id);  
  if (rank == 0) ta.write ("Analysis ");

  outfile << " after job = 1 on cpu " << rank << " INFO(1) = " <<  id.info[0] 
          << " INFO(2) = " <<  id.info[1] << endl;
  
  Timing tf1;  
  id.job = 2;  MPI_Barrier (MPI_COMM_WORLD);  dmumps_c (&id);  
  if (rank == 0) tf1.write ("Factorization(1) ");

  outfile << " after job = 2 on cpu " << rank << " INFO(1) = " <<  id.info[0] 
          << " INFO(2) = " <<  id.info[1] << endl;
    
  Timing ts1;  id.job = 3; 
  MPI_Barrier (MPI_COMM_WORLD);  dmumps_c (&id);
  if (rank == 0) ts1.write ("Solve(1) ");

  outfile << " after job = 3 on cpu " << rank << " INFO(1) = " <<  id.info[0] 
          << " INFO(2) = " <<  id.info[1] << endl;
  
  // 4) Solve system twice

  Timing tf2;
  id.job = 2;  MPI_Barrier (MPI_COMM_WORLD);  dmumps_c (&id);
  if (rank == 0)
    {
      Timing t0;
      ifstream in2(argv[2]);
      if (!in2)
        {
          cerr << "cannot open file " << argv[2] << endl;
          MPI_Finalize ();      return 4;
        }
      
      outfile << "reading rhs from file: " << argv[2] << endl;
      rhs.read (in2);
      
      t0.write ("Reading RHS and set up MUMPS");
    }
  MPI_Barrier (MPI_COMM_WORLD);

  if (rank == 0) tf2.write ("Factorization(2) ");
  outfile << " after job = 2 on cpu " << rank << " INFO(1) = " <<  id.info[0] 
          << " INFO(2) = " <<  id.info[1] << endl;
  
  Timing ts2;
  id.job = 3;  MPI_Barrier (MPI_COMM_WORLD);  dmumps_c (&id);  
  if (rank == 0) ts2.write ("Solve(2) ");

  outfile << " after job = 3 on cpu " << rank << " INFO(1) = " <<  id.info[0] 
          << " INFO(2) = " <<  id.info[1] << endl;
  
  // 5) write the solution out
  //solution is in rhs (it is overwritten)
  
  if (rank == 0)
    {
      Timing t0;
      cout << "Saving solution in " << argv[2] << ".solution" << endl;
      ofstream outf ((string (argv[2]) + ".solution").c_str ());
      outfile << " after solving the system twice " 
              << " RINFOG(4) = " << id.rinfog[3]
              << " RINFOG(5) = " << id.rinfog[4] 
              << " RINFOG(6) = " << id.rinfog[5] 
              << endl;
        
      rhs.write(outf);
      t0.write ("writing solution to file");
    }
  MPI_Barrier (MPI_COMM_WORLD);
  
  // 6) clean up
  Timing t2;
  MPI_Barrier (MPI_COMM_WORLD);  id.job = JOB_END;   dmumps_c (&id);  
  if (rank == 0) t2.write ("freeing memorly");

  outfile << " after job =-2 on cpu " << rank << " INFO(1) = " <<  id.info[0] 
          << " INFO(2) = " <<  id.info[1] << endl;
  
  MPI_Finalize ();  return 0;	
}

