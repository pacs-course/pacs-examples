
#ifndef HAVE_SPARSE_DISTRIBUTED_H
#define HAVE_SPARSE_DISTRIBUTED_H 1

#include <mpi.h>
#include <sparse_matrix_simplified.h>

class
distributed_sparse_matrix
  : public sparse_matrix
{

private :

  void
  non_local_csr ( );
  

  size_t is, ie;
  MPI_Comm comm;
  int mpirank, mpisize;
    
public :

  struct
  non_local_t
  {
    std::vector<int> row_ptr, col_ind;
    std::vector<double> a;
  } non_local;

  std::map<int, std::vector<int>> row_buffers;
  std::map<int, std::vector<int>> col_buffers;
  std::map<int, std::vector<double>> val_buffers;

  std::vector<int> ranges;
  std::vector<int> rank_nnz;

  void
  set_ranges (size_t is_, size_t ie_, MPI_Comm comm_)
  {
    is = is_; ie = ie_; comm = comm_;
    MPI_Comm_rank (comm, &mpirank);
    MPI_Comm_size (comm, &mpisize);
  }

  void
  assemble ();

};

#endif
