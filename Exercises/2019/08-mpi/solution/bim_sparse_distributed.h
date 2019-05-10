/*
  Copyright (C) 2018 Carlo de Falco
  This software is distributed under the terms
  the terms of the GNU/GPL licence v3
*/

#ifndef HAVE_BIM_SPARSE_DISTRIBUTED_H
#define HAVE_BIM_SPARSE_DISTRIBUTED_H 1

#include <mpi.h>
#include <bim_sparse.h>

class
distributed_sparse_matrix
  : public sparse_matrix
{

private :

  void
  non_local_csr ();

  int is, ie;
  MPI_Comm comm;
  int mpirank, mpisize;

  struct
  non_local_t
  {
    std::vector<int> prc_ptr, row_ind, col_ind;
    std::vector<double> a;
  } non_local;

  std::map<int, std::vector<int>> row_buffers;
  std::map<int, std::vector<int>> col_buffers;
  std::map<int, std::vector<double>> val_buffers;

  std::vector<int> ranges;
  std::vector<int> rank_nnz;

  bool mapped;

public :

  void
  set_ranges (int is_, int ie_);

  void
  set_ranges (int num_owned_);

  distributed_sparse_matrix (int is_, int ie_,
                             MPI_Comm comm_ = MPI_COMM_WORLD)
    : distributed_sparse_matrix ()
  { set_ranges (is_, ie_); }

  distributed_sparse_matrix (MPI_Comm comm_ = MPI_COMM_WORLD)
    : comm (comm_), mapped (false)
  { }

  void
  remap ();

  void
  assemble ();

  void
  csr (std::vector<double> &a,
       std::vector<int> &col,
       std::vector<int> &row,
       int base = 0,
       bool flag = false);

  void
  csr_update (std::vector<double> &a,
              const std::vector<int> &col_ind,
              const std::vector<int> &row_ptr,
              int base = 0,
              bool flag = false);

  void
  aij (std::vector<double> &a,
       std::vector<int> &i,
       std::vector<int> &j,
       int base = 0,
       bool flag = false);

  int
  owned_nnz ();

  int
  range_start ()
  { return is; };

  int
  range_end ()
  { return ie; };


};

#endif
