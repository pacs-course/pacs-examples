#include "sparse_matrix_assembly.h"


void
build_matrix (int rank, int size, int &is, int &ie, sparse_matrix &A)
{
  A.resize (msize);
  
  if (rank == 0 || size == 1)
    {
      is = 0;
      ie = 2;

      A[0][0] += 4; A[0][1] +=-2;      
      A[1][0] +=-2; A[1][1] += 5; A[1][2] +=-1;
                    A[2][1] +=-1;
    }
   if (rank == 1 || size == 1)
    {
      is = 2;
      ie = 4;

      A[1][0] +=-1;               A[1][2] +=-1;
                    A[2][1] +=-1; A[2][2] += 4; A[2][3] +=-2;
                                  A[3][2] +=-2; A[3][3] += 4; A[3][4] += -1; 
                                                A[4][3] +=-1;            
    }
   if (rank == 2 || size == 1)
    {
      is = 4;
      ie = 6;
                                                              A[3][4] +=-1;
                                                A[4][3] +=-1; A[4][4] += 4; A[4][5] += -2;
                                                              A[5][4] +=-2; A[5][5] +=  4;
    }

   if (size == 1)
     {  is = 0; ie = 6; }
   
}

void
non_local_t::csr (sparse_matrix &A, int is, int ie)
{
  A.set_properties ();
  a.reserve (A.nnz); col_ind.reserve (A.nnz); // FIXME: ESAGERATO!!! meglio "nnz / mpisize" ??
  row_ptr.resize (A.rows () + 1);

  int idx = 0, idr = 0;

  sparse_matrix::col_iterator jj;

  for (size_t ii = 0; ii < A.size (); ++ii)
    {
      row_ptr[idr++] = idx;
      if ((A[ii].size () > 0) && ((ii < is) || (ii >= ie)))
        {
          for (jj  = A[ii].begin (); jj != A[ii].end (); ++jj)
            {
              col_ind.push_back (A.col_idx (jj));
              a.push_back (A.col_val (jj));
              idx++;
            }
        }
    }
  std::fill (row_ptr.begin () + idr, row_ptr.end (), col_ind.size ());
};  
