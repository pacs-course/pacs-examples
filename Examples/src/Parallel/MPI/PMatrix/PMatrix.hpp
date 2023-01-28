/*
 * PMatrix.hpp
 *
 *  Created on: Oct 15, 2022
 *      Author: forma
 */

#ifndef AMSC_EXAMPLES_EXAMPLES_SRC_PARALLEL_MPI_PMATRIX_PMATRIX_HPP_
#define AMSC_EXAMPLES_EXAMPLES_SRC_PARALLEL_MPI_PMATRIX_PMATRIX_HPP_
#include "Matrix.hpp"      // in Matrix/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-override"
#pragma GCC diagnostic ignored "-Wcast-function-type"
#include "mpi_utils.hpp"   // for MPI_SIZE_T and mpi_typeof()
#include "partitioner.hpp" // in Parallel/Utilities
#include <mpi.h>
#pragma GCC diagnostic pop
#include <array>
#include <vector>
namespace apsc
{
/*!
 * A class for parallel matrix product
 * @tparam Matrix A matrix compliant with that in Matrix.hpp
 */
template <class Matrix> class PMatrix
{
public:
  /*!
   * We assume that Matrix defines a type equal to that of the contained element
   */
  using Scalar = typename Matrix::Scalar;
  /*!
   * All processes will call setup but only the managing process
   * (with number 0) will actually pass a non-empty global matrix
   *
   * @param gMat The global matrix
   * @param communic The MPI communicator
   *
   * @note This is not the only way to create a parallel matrix. Alternatively,
   * all processes may directly build the local matrix, for instance reading
   * from a file. In this case, for the setup we need just the number of rows
   * and columns of the global matrix
   */
  void setup(Matrix const &gMat, MPI_Comm communic);

  /*!
   * Performs the local matrix times vector product.
   * For simplicity we do not partition the input vector, which is indeed
   * a global vector.
   *
   * @param x A global vector
   */
  void product(std::vector<Scalar> const &x);
  /*!
   * Gets the global solution. All processes call it but just process 0
   * (manager) gets a non empty vector equal to the result
   *
   * @return the global solution of the matrix product (only process 0), in v.
   */
  void collectGlobal(std::vector<Scalar> &v) const;
  /*!
   * Gets the global solution. All processes call it and get the
   * result.
   *
   * @return the global solution of the matrix product, in v.
   */
  void AllCollectGlobal(std::vector<Scalar> &v) const;
  /*!
   * Returns the local matrix assigned to the processor
   * @return The local matrix
   */
  auto const &
  getLocalMatrix() const
  {
    return localMatrix;
  }
  static constexpr int manager = 0;

protected:
  MPI_Comm         mpi_comm;
  int              mpi_rank;      // my rank
  int              mpi_size;      // the number of processes
  std::vector<int> counts;        // The vector used for gathering/scattering
  std::vector<int> displacements; // The vector used for gathering/scattering
  Matrix           localMatrix;   // The local portion of the matrix
  std::vector<Scalar> localProduct; // The place where to store the result of the local mult.
  std::size_t local_nRows = 0u;
  std::size_t local_nCols = 0u;
  std::size_t global_nRows = 0u;
  std::size_t global_nCols = 0u;
  // std::size_t offset_Row=0u;
  // std::size_t offset_Col=0u;
  // I use mpi_typeof() in mpi_util.h to recover genericity. Note that to
  // activate the overload I need to create an object of type Scalar. I use the
  // default constructor, withScalar{}
  MPI_Datatype MPI_Scalar_Type = mpi_typeof(Scalar{});
};
} // end namespace apsc

template <class Matrix>
void
apsc::PMatrix<Matrix>::setup(const Matrix &gMat, MPI_Comm communic)
{
  mpi_comm = communic;
  MPI_Comm_rank(mpi_comm, &mpi_rank);
  MPI_Comm_size(mpi_comm, &mpi_size);
  using namespace apsc::LinearAlgebra;
  // This will contain the number of row and columns of all the local matrices
  std::array<std::vector<std::size_t>, 2> localRandC;
  localRandC[0].resize(
    mpi_size); // get the right size to avoid messing up things
  localRandC[1].resize(mpi_size);
  // We need the tools to split the matrix data buffer
  counts.resize(mpi_size);
  displacements.resize(mpi_size);
  if(mpi_rank == manager)
    {
      // I am the boss
      global_nRows = gMat.rows();
      global_nCols = gMat.cols();
    }
  // it would be more efficient to pack stuff to be broadcasted
  MPI_Bcast(&global_nRows, 1, MPI_SIZE_T, manager, mpi_comm);
  MPI_Bcast(&global_nCols, 1, MPI_SIZE_T, manager, mpi_comm);

  // I use for the partitioning the same block distribution type as the ordering
  // of the matrix: matrices in ROWMAJOR storage will be partitioned along rows
  // and viceversa
  constexpr apsc::ORDERINGTYPE P_ORDERING =
    Matrix::ordering == ORDERING::ROWMAJOR ? apsc::ORDERINGTYPE::ROWWISE
                                           : apsc::ORDERINGTYPE::COLUMNWISE;

  // I let all tasks compute the partition data, alternative
  // is have it computed only by the master rank and then
  // broadcast. But remember that communication is costly.

  MatrixPartitioner<apsc::DistributedPartitioner, P_ORDERING> partitioner(
    global_nRows, global_nCols, mpi_size); // the partitioner

  auto countAndDisp = apsc::counts_and_displacements(partitioner);
  counts = countAndDisp[0];
  displacements = countAndDisp[1];
  localRandC = partitioner.getLocalRowsAndCols(mpi_size);
  local_nRows = localRandC[0][mpi_rank];
  local_nCols = localRandC[1][mpi_rank];

  // Now get the local matrix!
  localMatrix.resize(local_nRows, local_nCols);
  int matrixSize = local_nRows * local_nCols;
  MPI_Scatterv(gMat.data(), counts.data(), displacements.data(),
               MPI_Scalar_Type, localMatrix.data(), matrixSize, MPI_Scalar_Type,
               manager, mpi_comm);
}

template <class Matrix>
void
apsc::PMatrix<Matrix>::product(const std::vector<Scalar> &x)
{
  using namespace apsc::LinearAlgebra;
  if constexpr(Matrix::ordering == ORDERING::ROWMAJOR)
    {
      // this is the simplest case. The matrix has all column
      // (but not all rows!)
      this->localProduct = this->localMatrix * x;
    }
  else
    {
      //  This case is much trickier. The local matrix has fewer columns than
      //  the rows of x,
      // so I need to reduce the global vector in input x
      // I need to get the portion of x corresponding to the columns in the
      // global matrix
      auto startcol = displacements[mpi_rank] / global_nRows;
      auto endcol = (displacements[mpi_rank] + counts[mpi_rank]) / global_nRows;
      // I copy the relevant portion of the global vector in a local vector
      // exploiting a constructor that takes a range.
      // A more elegant, efficient (but complicated) version would be to use a
      // view. It could have been done if I've used Eigen matrices instead of
      // vectors.
      std::vector<Scalar> y(x.begin() + startcol, x.begin() + endcol);
      this->localProduct = this->localMatrix * y;
    }
}

template <class Matrix>
void
apsc::PMatrix<Matrix>::collectGlobal(std::vector<Scalar> &v) const
{
  using namespace apsc::LinearAlgebra;
  if(mpi_rank == manager)
    v.resize(global_nRows);
  if constexpr(Matrix::ordering == ORDERING::ROWMAJOR)
    {
      // I need to gather the contribution, but first I need to have
      // find the counts and displacements for the vector
      std::vector<int> vec_counts(mpi_size);
      std::vector<int> vec_displacements(mpi_size);
      for(int i = 0; i < mpi_size; ++i)
        {
          vec_counts[i] = counts[i] / global_nCols;
          vec_displacements[i] = displacements[i] / global_nCols;
        }
      // note: vec_counts[i] should be equal to the number of local matrix rows.
      MPI_Gatherv(localProduct.data(), localProduct.size(), MPI_Scalar_Type,
                  v.data(), vec_counts.data(), vec_displacements.data(),
                  MPI_Scalar_Type, manager, mpi_comm);
    }
  else
    {
      // I need to do a reduction
      // The local vectors are of the richt size, but contain only
      // a partial result. I need to sum up.
      /*
       * int MPI_Reduce(const void* send_buffer,
               void* receive_buffer,
               int count,
               MPI_Datatype datatype,
               MPI_Op operation,
               int root,
               MPI_Comm communicator);
       */
      MPI_Reduce(localProduct.data(), v.data(), global_nRows, MPI_Scalar_Type,
                 MPI_SUM, manager, mpi_comm);
    }
}

template <class Matrix>
void
apsc::PMatrix<Matrix>::AllCollectGlobal(std::vector<Scalar> &v) const
{
  using namespace apsc::LinearAlgebra;
  v.resize(global_nRows);
  if constexpr(Matrix::ordering == ORDERING::ROWMAJOR)
    {
      // I need to gather the contribution, but first I need to have
      // find the counts and displacements for the vector
      std::vector<int> vec_counts(mpi_size);
      std::vector<int> vec_displacements(mpi_size);
      for(int i = 0; i < mpi_size; ++i)
        {
          vec_counts[i] = counts[i] / global_nCols;
          vec_displacements[i] = displacements[i] / global_nCols;
        }
      MPI_Allgatherv(localProduct.data(), localProduct.size(), MPI_Scalar_Type,
                     v.data(), vec_counts.data(), vec_displacements.data(),
                     MPI_Scalar_Type, mpi_comm);
    }
  else
    {
      //  This case is trickier. I need to do a reduction
      MPI_Allreduce(localProduct.data(), v.data(), global_nRows,
                    MPI_Scalar_Type, MPI_SUM, mpi_comm);
    }
}

#endif /* AMSC_EXAMPLES_EXAMPLES_SRC_PARALLEL_MPI_PMATRIX_PMATRIX_HPP_ */
