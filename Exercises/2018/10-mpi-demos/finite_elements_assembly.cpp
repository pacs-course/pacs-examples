#include <mpi.h>

#include "sparse_distributed.h"

int
conn (int inode, int iel)
{
  int res = 0;
  int elcol, elrow;
  elcol = iel / 4;
  elrow = iel - elcol * 4;
  res = elcol * 5 + elrow;
  if (inode == 1) res += 1;
  if (inode == 2) res += 5;
  if (inode == 3) res += 6;
  return (res);
};

int
main (int argc, char *argv[])
{
  MPI_Init (&argc, &argv);

  int rank, size;
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Comm_size (MPI_COMM_WORLD, &size);

  int is, ie, is_elems, ie_elems;
  distributed_sparse_matrix A;
  std::vector<int> row_ptr, col_ind;
  std::vector<double> value;
  
  A.resize (55);
  
  if (rank == 0 || size == 1)
    {
      is_elems = 0;
      ie_elems = 20;
      is = 0;
      ie = 28;
    }

   if (rank == 1 || size == 1)
     {
      is_elems = 20;
      ie_elems = 40;
      is = 28;
      ie = 55;
     }

   A.set_ranges (is, ie, MPI_COMM_WORLD);
   std::cout << "is " << is << " ie " << ie << " is_elems " << is_elems << "  ie_elems " << ie_elems << std::endl;  
     
   if (size == 1)
     { std::cout << "runing serially\n" ; is_elems = 0; ie_elems = 40; is = 0; ie = 55; }
   
   std::vector<std::vector<double>> locmatrix =
     {{2,-1,-1,0}, {-1,2,0,-1},{-1,0,2,-1},{0,-1,-1,2}};

   double tmp;
   for (int iel = is_elems; iel < ie_elems; ++iel)
     for (int inode = 0; inode < 4; ++inode)
       for (int jnode = 0; jnode < 4; ++jnode)
         {
           tmp = locmatrix[inode][jnode];
           if (tmp != 0.)
             A[conn (inode,iel)][conn (jnode,iel)] +=
               tmp;
         }

   A.assemble ();
   A.csr (value, col_ind, row_ptr, 0);
  
  for (int ii = 0; ii < size; ++ii)
    {
      if (ii == rank)
        {
          std::cout << "## rank " << rank << std::endl;

          std::cout << A << std::endl;
           std::cout << "\n\n";
           
          std::cout << "a = [";
          for (auto ii : A.non_local.a)
            std::cout << ii << " ";
          std::cout << "];\n";

          std::cout << "col_ind = [";
          for (auto ii : A.non_local.col_ind)
            std::cout << ii << " ";
          std::cout << "];\n";

          std::cout << "row_ptr = [";
          for (auto ii : A.non_local.row_ptr)
            std::cout << ii << " ";          
          std::cout << "];\n";
          
          std::cout << "ranges = [";
          for (auto ii : A.ranges)
            std::cout << ii << " ";
          std::cout << "];\n\n";

          std::cout << "rank_nnz = [";
          for (auto ii : A.rank_nnz)
            std::cout << ii << " ";
          std::cout << "];\n\n";

          
          for (auto ii : A.row_buffers)
            {
              std::cout << "row_buffers (" << ii.first << ") = [";
                for (auto jj : ii.second)
                  std::cout << jj << " ";
              std::cout << "];\n";
            }

          for (auto ii : A.col_buffers)
            {
              std::cout << "col_buffers (" << ii.first << ") = [";
                for (auto jj : ii.second)
                  std::cout << jj << " ";
              std::cout << "];\n";
            }

          for (auto ii : A.val_buffers)
            {
              std::cout << "val_buffers (" << ii.first << ") = [";
                for (auto jj : ii.second)
                  std::cout << jj << " ";
              std::cout << "];\n";
            }

        }

      MPI_Barrier (MPI_COMM_WORLD);
    }
  
  MPI_Finalize ();
  return 0;
}
