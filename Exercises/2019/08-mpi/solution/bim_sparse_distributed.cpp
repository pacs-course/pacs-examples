/*
  Copyright (C) 2018 Carlo de Falco
  This software is distributed under the terms
  the terms of the GNU/GPL licence v3
*/


#include <bim_sparse_distributed.h>
#include <mpi_alltoallv2.h>

void
distributed_sparse_matrix::set_ranges (int is_, int ie_)
{
  is = is_; ie = ie_;
  MPI_Comm_rank (comm, &mpirank);
  MPI_Comm_size (comm, &mpisize);

  /// Gather ranges
  ranges.assign (mpisize + 1, 0);
  MPI_Allgather (&ie, 1, MPI_INT, &(ranges[1]), 1, MPI_INT, comm);
  this->resize (ranges.back ());
}

void
distributed_sparse_matrix::set_ranges (int num_owned_)
{
  MPI_Comm_size (comm, &mpisize);
  MPI_Comm_rank (comm, &mpirank);

  /// Gather ranges
  ranges.assign (mpisize + 1, 0);
  MPI_Allgather (&num_owned_, 1, MPI_INT, &(ranges[1]),
                 1, MPI_INT, comm);
  for (auto irank = 0; irank < mpisize; ++irank)
    ranges[irank+1] += ranges[irank];

  is = ranges[mpirank];
  ie = ranges[mpirank+1];
  this->resize (ranges.back ());
}

void
distributed_sparse_matrix::non_local_csr ()
{
  non_local.row_ind.clear ();
  non_local.col_ind.clear ();
  non_local.a.clear ();
  this->set_properties ();

  non_local.prc_ptr.assign (this->mpisize + 1, 0);
  sparse_matrix::col_iterator jj;

  for (auto ii = 0; ii < this->mpisize; ++ii)
    {
      non_local.prc_ptr[ii+1] = non_local.prc_ptr[ii];
      if (ii != mpirank)
        for (auto kk = ranges[ii]; kk < ranges[ii+1]; ++kk)
          {
            non_local.prc_ptr[ii+1] += (*this)[kk].size ();
            non_local.col_ind.reserve (non_local.col_ind.size ()
                                       + (*this)[kk].size ());
            non_local.row_ind.reserve (non_local.row_ind.size ()
                                       + (*this)[kk].size ());
            non_local.a.reserve (non_local.a.size ()
                                 + (*this)[kk].size ());
            for (jj  = (*this)[kk].begin ();
                 jj != (*this)[kk].end (); ++jj)
              {
                non_local.row_ind.push_back (kk);
                non_local.col_ind.push_back (this->col_idx (jj));
                non_local.a.push_back (this->col_val (jj));
              }
          }
    }

  non_local.a.resize (non_local.row_ind.size ());
  for (int j = 0; j < non_local.col_ind.size (); ++j)
    non_local.a[j] =
      (*this)[non_local.row_ind[j]][non_local.col_ind[j]];
}


int
distributed_sparse_matrix::owned_nnz ()
{
  int retval = 0;
  for (int ii = is; ii < ie; ++ii)
    retval += (*this)[ii].size ();
  return retval;
}

void
distributed_sparse_matrix::remap ()
{
  non_local_csr ();

  /// Distribute buffer sizes
  rank_nnz.assign (mpisize, 0);
  for (int ii = 0; ii < mpisize; ++ii)
    rank_nnz[ii] = non_local.prc_ptr[ii+1] - non_local.prc_ptr[ii];
  MPI_Alltoall (MPI_IN_PLACE, 1, MPI_INT, &(rank_nnz[0]),
                1, MPI_INT, comm);

  /// Allocate buffers
  for (int ii = 0; ii < mpisize; ++ii)
    if ((rank_nnz[ii] > 0) && (ii != mpirank))
      {
        row_buffers[ii].resize (rank_nnz[ii]);
        col_buffers[ii].resize (rank_nnz[ii]);
        val_buffers[ii].resize (rank_nnz[ii]);
      }


  /// Communicate overlap regions
  void * sendbuf[mpisize];
  int sendcnts[mpisize];
  void * recvbuf[mpisize];
  int recvcnts[mpisize];
  
  /// 1) communicate row_ptr
  for (int ii = 0; ii < mpisize; ++ii)
    {
      recvbuf[ii]  = &(row_buffers[ii][0]);
      recvcnts[ii] =   row_buffers[ii].size ();
      sendbuf[ii]  = &(non_local.row_ind[non_local.prc_ptr[ii]]);
      sendcnts[ii] =   non_local.prc_ptr[ii+1] -
        non_local.prc_ptr[ii];
    }

  MPI_Alltoallv2 (sendbuf, sendcnts, MPI_INT,
                  recvbuf, recvcnts, MPI_INT, comm);

  /// 2) communicate col_ind
  for (int ii = 0; ii < mpisize; ++ii)
    {
      recvbuf[ii]  = &(col_buffers[ii][0]);
      recvcnts[ii] =   col_buffers[ii].size ();
      sendbuf[ii]  = &(non_local.col_ind[non_local.prc_ptr[ii]]);
      sendcnts[ii] =   non_local.prc_ptr[ii+1] -
        non_local.prc_ptr[ii];
    }

  MPI_Alltoallv2 (sendbuf, sendcnts, MPI_INT,
                  recvbuf, recvcnts, MPI_INT, comm);
  
  mapped = true;
}


void
distributed_sparse_matrix::assemble ()
{

  if (! mapped)
    remap ();

  /// 3) communicate values
  void * sendbuf[mpisize];
  int sendcnts[mpisize];
  void * recvbuf[mpisize];
  int recvcnts[mpisize];

   for (int ii = 0; ii < mpisize; ++ii)
    {
      recvbuf[ii]  = &(val_buffers[ii][0]);
      recvcnts[ii] =   val_buffers[ii].size ();
      sendbuf[ii]  = &(non_local.a[non_local.prc_ptr[ii]]);
      sendcnts[ii] =   non_local.prc_ptr[ii+1] -
        non_local.prc_ptr[ii];
    }

  MPI_Alltoallv2 (sendbuf, sendcnts, MPI_DOUBLE,
                  recvbuf, recvcnts, MPI_DOUBLE, comm);

  /// 4) insert communicated values into sparse_matrix
  for (int ii = 0; ii < mpisize; ++ii) // loop over ranks
    if (ii != mpirank)
      for (int kk = 0; kk < rank_nnz[ii]; ++kk)
        (*this)[row_buffers[ii][kk]][col_buffers[ii][kk]]
          += val_buffers[ii][kk];


  /// 5) zero out communicated values
  for (int iprc = 0; iprc < mpisize; ++iprc)
    if (iprc != mpirank)
      {
        for (int ii = non_local.prc_ptr[iprc];
             ii < non_local.prc_ptr[iprc+1]; ++ii)
          // the following will throw if we try to access
          // an element which does not exist yet!
          (*this)[non_local.row_ind[ii]].at
            (non_local.col_ind[ii]) = 0.0;
      }

}

void
distributed_sparse_matrix::csr (std::vector<double> &a,
                                std::vector<int> &col,
                                std::vector<int> &row,
                                int base,
                                bool flag)
{

  if (flag == false)
    {
      a.resize (owned_nnz ());
      col.resize (a.size ());

      row.resize (ie - is + 1);

      int idx = 0;
      int idr = 0;
      typename sparse_matrix::col_iterator jj;
      for (auto ii = is; ii < ie; ++ii)
        {
          row[idr] = idx + base;

          if ((*this)[ii].size () > 0)
            {
              for (jj  = (*this)[ii].begin ();
                   jj != (*this)[ii].end (); ++jj)
                {
                  col[idx] = this->col_idx (jj) + base;
                  a[idx] = this->col_val (jj);
                  idx++;
                }
            }
          idr++;

        }

      std::fill (row.begin () + idr, row.end (), idx + base);
    }
  else
    this->sparse_matrix::csr (a, col, row, base);

}

void
distributed_sparse_matrix::csr_update
(std::vector<double> &a,
 const std::vector<int> &col_ind,
 const std::vector<int> &row_ptr,
 int base,
 bool flag)
{
  if (! flag)
    {
      auto ni = row_ptr.size ();
      auto nj = col_ind.size ();
      a.resize (nj);
      int idx = 0;
  
      for (auto in = 0; in < ni - 1; ++in)
        for (auto jn = row_ptr[in] - base;
             jn < row_ptr[in+1] - base; ++jn)
          a[idx++] = (*this)[in + is][col_ind[jn] - base];
    }
  else
    this->sparse_matrix::csr_update (a, col_ind, row_ptr, base);
}

void
distributed_sparse_matrix::aij (std::vector<double> &a,
                                std::vector<int> &i,
                                std::vector<int> &j,
                                int base,
                                bool flag)
{
  if (! flag)
    {
      a.resize (owned_nnz ());
      i.resize (a.size ());
      j.resize (a.size ());

      int idx = 0;
      sparse_matrix::col_iterator jj;

      for (size_t ii = is; ii < ie; ++ii)
        if ((*this)[ii].size ())
          for (jj  = (*this)[ii].begin (); jj != (*this)[ii].end (); ++jj)
            {
              i[idx] = ii+base;
              j[idx] = this->col_idx (jj)+base;
              a[idx] = this->col_val (jj);
              idx++;
            }
    }
  else
    this->sparse_matrix::aij (a, i, j, base);
}
