#include <mpi.h>

#include "sparse_matrix_assembly.h"

non_local_t non_local;

int
main (int argc, char *argv[])
{
  MPI_Init (&argc, &argv);

  int rank, size;
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Comm_size (MPI_COMM_WORLD, &size);

  int is, ie;
  sparse_matrix A;

  build_matrix (rank, size, is, ie, A);
  int nloc = ie - is;

    /*
      for (auto irank = 0; irank < size; ++irank)
      {
      if (irank == rank)
      {
      std::cout << A << std::endl;
      }
      MPI_Barrier (MPI_COMM_WORLD);
      }
  
      
      MPI_Finalize ();
      return 0;
    */
  
  std::vector<int> row_ptr, col_ind;
  std::vector<double> value;
  
  /// Gather ranges
  std::vector<int> ranges (size + 1, 0);
  MPI_Allgather (&ie, 1, MPI_INT, &(ranges[1]), 1, MPI_INT, MPI_COMM_WORLD);
  non_local.csr (A, is, ie);
  
  /// Distribute buffer sizes
  std::vector<int> rank_nnz (size, 0);
  for (int ii = 0; ii < size; ++ii)
    rank_nnz[ii] = non_local.row_ptr[ranges[ii+1]] - non_local.row_ptr[ranges[ii]];
  MPI_Alltoall (MPI_IN_PLACE, 1, MPI_INT, &(rank_nnz[0]), 1, MPI_INT, MPI_COMM_WORLD);

      
  /// Allocate buffers
  std::map<int, std::vector<int>> row_buffers;
  std::map<int, std::vector<int>> col_buffers;
  std::map<int, std::vector<double>> val_buffers;
  for (int ii = 0; ii < size; ++ii)
    if ((rank_nnz[ii] > 0) && (ii != rank))
      {
        row_buffers[ii].resize (ranges[rank+1] - ranges[rank] + 1);
        col_buffers[ii].resize (rank_nnz[ii]);
        val_buffers[ii].resize (rank_nnz[ii]);
      }

  
  /// Communicate overlap regions
  
  /// 1) communicate row_ptr
  std::vector<MPI_Request> reqs;
  for (int ii = 0; ii < size; ++ii)
    {
      if (ii == rank) continue; // No communication to self!
      if (rank_nnz[ii] > 0) // we must receive something from rank ii
        {
          int recv_tag = ii   + size * rank;
          reqs.resize (reqs.size () + 1);
          MPI_Irecv (&(row_buffers[ii][0]), row_buffers[ii].size (), MPI_INT, ii, recv_tag, MPI_COMM_WORLD, &(reqs.back ()));
        }
      if (non_local.row_ptr[ranges[ii+1]] > non_local.row_ptr[ranges[ii]]) // we must send something to rank ii
        {
          int send_tag = rank + size * ii;
          reqs.resize (reqs.size () + 1);
          MPI_Isend (&(non_local.row_ptr[ranges[ii]]), ranges[ii+1] - ranges[ii] + 1,
                     MPI_INT, ii, send_tag, MPI_COMM_WORLD, &(reqs.back ()));
        }     
      }
  
  MPI_Waitall (reqs.size (), &(reqs[0]), MPI_STATUSES_IGNORE);
  reqs.clear ();
    
  /// 2) communicate col_ind
  for (int ii = 0; ii < size; ++ii)
    {
      if (ii == rank) continue; // No communication to self!
      if (rank_nnz[ii] > 0) // we must receive something from rank ii
        {
          int recv_tag = ii   + size * rank;
          reqs.resize (reqs.size () + 1);
          MPI_Irecv (&(col_buffers[ii][0]), col_buffers[ii].size (), MPI_INT, ii, recv_tag, MPI_COMM_WORLD, &(reqs.back ()));
        }
      if (non_local.row_ptr[ranges[ii+1]] > non_local.row_ptr[ranges[ii]]) // we must send something to rank ii
        {
          int send_tag = rank + size * ii;
          reqs.resize (reqs.size () + 1);
          MPI_Isend (&(non_local.col_ind[non_local.row_ptr[ranges[ii]]]),
                     non_local.row_ptr[ranges[ii+1]] - non_local.row_ptr[ranges[ii]], MPI_INT, ii, send_tag, MPI_COMM_WORLD, &(reqs.back ()));
        }     
      }
  MPI_Waitall (reqs.size (), &(reqs[0]), MPI_STATUSES_IGNORE);
  reqs.clear ();
    
  /// 3) communicate values
  for (int ii = 0; ii < size; ++ii)
    {
      if (ii == rank) continue; // No communication to self!
      if (rank_nnz[ii] > 0) // we must receive something from rank ii
        {
          int recv_tag = ii   + size * rank;
          reqs.resize (reqs.size () + 1);
          MPI_Irecv (&(val_buffers[ii][0]), val_buffers[ii].size (), MPI_DOUBLE, ii, recv_tag, MPI_COMM_WORLD, &(reqs.back ()));
        }
      if (non_local.row_ptr[ranges[ii+1]] > non_local.row_ptr[ranges[ii]]) // we must send something to rank ii
        {
          int send_tag = rank + size * ii;
          reqs.resize (reqs.size () + 1);
          MPI_Isend (&(non_local.a[non_local.row_ptr[ranges[ii]]]),
                     non_local.row_ptr[ranges[ii+1]] - non_local.row_ptr[ranges[ii]], MPI_DOUBLE, ii, send_tag, MPI_COMM_WORLD, &(reqs.back ()));
        }     
      }
  MPI_Waitall (reqs.size (), &(reqs[0]), MPI_STATUSES_IGNORE);
  reqs.clear ();
  
  /// 4) insert communicated values into sparse_matrix
  for (int ii = 0; ii < size; ++ii) // loop over ranks
    if ((rank_nnz[ii] > 0) && (ii != rank))
      for (int jj = 0; jj < ranges[rank+1] - ranges[rank]; ++jj) // loop over rows of rank
        for (int kk = row_buffers[ii][jj] - row_buffers[ii][0];
             kk < row_buffers[ii][jj+1]   - row_buffers[ii][0];
             ++kk) // loop over columns of row
          A[jj + is][col_buffers[ii][kk]] += val_buffers[ii][kk];

  /// 5) zero out communicated values  
  for (int irow = 0; irow < is; ++irow)
    if (A[irow].size ())
      for (auto& jj : A[irow])
        jj.second = 0.0;

  for (int irow = ie; irow < A.size (); ++irow)
    if (A[irow].size ())
      for (auto& jj : A[irow])
        jj.second = 0.0;


  A.csr (value, col_ind, row_ptr, 0);
  

  for (int ii = 0; ii < size; ++ii)
    {
      if (ii == rank)
        {
          std::cout << "## rank " << rank << std::endl;

          std::cout << A << std::endl;
           std::cout << "\n\n";
           
          std::cout << "a = [";
          for (auto ii : non_local.a)
            std::cout << ii << " ";
          std::cout << "];\n";

          std::cout << "col_ind = [";
          for (auto ii : non_local.col_ind)
            std::cout << ii << " ";
          std::cout << "];\n";

          std::cout << "row_ptr = [";
          for (auto ii : non_local.row_ptr)
            std::cout << ii << " ";          
          std::cout << "];\n";
          
          std::cout << "ranges = [";
          for (auto ii : ranges)
            std::cout << ii << " ";
          std::cout << "];\n\n";

          std::cout << "rank_nnz = [";
          for (auto ii : rank_nnz)
            std::cout << ii << " ";
          std::cout << "];\n\n";


          for (auto ii : row_buffers)
            {
              std::cout << "row_buffers (" << ii.first << ") = [";
                for (auto jj : ii.second)
                  std::cout << jj << " ";
              std::cout << "];\n";
            }

          for (auto ii : col_buffers)
            {
              std::cout << "col_buffers (" << ii.first << ") = [";
                for (auto jj : ii.second)
                  std::cout << jj << " ";
              std::cout << "];\n";
            }

          for (auto ii : val_buffers)
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
