#include <mpi.h>
#include <omp.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

static double c_start, c_diff;
#define tic() c_start = MPI_Wtime();
#define toc(x)                                       \
  {                                                  \
    c_diff = MPI_Wtime() - c_start;                  \
    std::cout << x << c_diff << " [s]" << std::endl; \
  }

/**
 * Parallel matrix-vector product.
 *
 * Data are generated randomly by rank 0 and then broadcasted. Each rank is
 * assigned a sub-block of the input matrix (split by rows, without overlap) and
 * computes the local matrix-vector product. Finally, the results are collected
 * back by rank 0, which prints them to output.
 *
 * This example makes use of hybrid shared/distributed parallelization through
 * OpenMP and MPI.
 *
 * Running, e.g., on a matrix of size 10^4 x 10^3 should show good scalability
 * properties.
 *
 * The bottleneck in the computational time is constructing the matrix on rank 0
 * and scattering it to other ranks: in real applications each rank would build
 * its own "local" matrix.
 */
int
main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);

  MPI_Comm mpi_comm = MPI_COMM_WORLD;

  int mpi_rank;
  MPI_Comm_rank(mpi_comm, &mpi_rank);

  int mpi_size;
  MPI_Comm_size(mpi_comm, &mpi_size);

#pragma omp parallel master
  if (mpi_rank == 0)
    std::cout << "Number of processes: " << mpi_size
              << ", number of threads: " << omp_get_num_threads() << std::endl;

  // Set to true to print matrix, vector and result.
  const bool print = false;

  unsigned int n_rows;
  unsigned int n_cols;

  std::vector<double> matrix;
  std::vector<double> rhs;
  std::vector<double> result;

  // Vectors to store the number of elements to send to each
  // processor and the offset index where to start reading them from.
  std::vector<int> send_counts;
  std::vector<int> send_start_idx;

  // Vectors to store the number of elements to receive from each
  // processor and the offset index where to start writing them into.
  std::vector<int> recv_counts;
  std::vector<int> recv_start_idx;

  if (mpi_rank == 0)
    {
      std::cout << std::endl << "Enter the number of matrix rows:" << std::endl;
      std::cin >> n_rows;

      if (n_rows < 1)
        {
          std::cerr << "ERROR: Number of rows should be greater than 1."
                    << std::endl;

          return 1;
        }

      std::cout << "Enter the number of matrix columns:" << std::endl;
      std::cin >> n_cols;

      if (n_cols < 1)
        {
          std::cerr << "ERROR: Number of columns should be greater than 1."
                    << std::endl;

          return 1;
        }

      std::cout << std::endl;
    }

  MPI_Bcast(&n_rows, 1, MPI_INT, 0, mpi_comm);
  MPI_Bcast(&n_cols, 1, MPI_INT, 0, mpi_comm);

  rhs.resize(n_cols);
  if (mpi_rank == 0)
    result.resize(n_rows);

  const unsigned int count     = n_rows / mpi_size;
  const int          remainder = n_rows - count * mpi_size;

  const unsigned int n_rows_local =
    (mpi_rank < remainder) ? (count + 1) : count;

  std::cout << "Number of rows on rank " << mpi_rank << ": " << n_rows_local
            << std::endl;

  std::vector<double> matrix_local(n_rows_local * n_cols);

  std::vector<double> result_local(n_rows_local, 0.0);

  if (mpi_rank == 0)
    {
      tic();

      std::random_device                     engine;
      std::uniform_real_distribution<double> rand(0, 1);

      // Generate matrix.
      matrix.resize(n_rows * n_cols);
#pragma omp parallel for shared(matrix)
      for (auto &m : matrix)
        m = rand(engine);

      if (print)
        {
          std::cout << "Matrix:" << std::endl;
          for (unsigned int i = 0; i < n_rows; ++i)
            {
              for (unsigned int j = 0; j < n_cols; ++j)
                std::cout << std::setw(12) << matrix[i * n_cols + j];

              std::cout << std::endl;
            }
          std::cout << std::endl;
        }

#pragma omp parallel for shared(rhs)
      // Generate rhs.
      for (auto &v : rhs)
        v = rand(engine);

      if (print)
        {
          std::cout << "RHS:" << std::endl;
          for (const auto &v : rhs)
            {
              std::cout << std::setw(12) << v;
            }
          std::cout << std::endl << std::endl;
        }

      send_counts.resize(mpi_size);
      send_start_idx.resize(mpi_size);
      recv_counts.resize(mpi_size);
      recv_start_idx.resize(mpi_size);

      int start_idx = 0;
      for (int i = 0; i < mpi_size; ++i)
        {
          recv_counts[i] = (i < remainder) ? (count + 1) : count;
          send_counts[i] = recv_counts[i] * n_cols;

          recv_start_idx[i] = start_idx;
          send_start_idx[i] = start_idx * n_cols;

          start_idx += recv_counts[i];
        }

      std::cout << std::endl;
      toc("Assemble matrix: time elapsed on rank " + std::to_string(mpi_rank) +
          ": ");
    }

  tic();
  MPI_Bcast(rhs.data(), rhs.size(), MPI_DOUBLE, 0, mpi_comm);

  MPI_Scatterv(matrix.data(),
               send_counts.data(),
               send_start_idx.data(),
               MPI_DOUBLE,
               matrix_local.data(),
               matrix_local.size(),
               MPI_DOUBLE,
               0,
               mpi_comm);

  MPI_Barrier(mpi_comm);
  for (int rank = 0; rank < mpi_size; ++rank)
    {
      if (rank == mpi_rank && mpi_rank == 0)
        std::cout << std::endl;
      MPI_Barrier(mpi_comm);

      if (rank == mpi_rank)
        {
          toc("Scatter: time elapsed on rank " + std::to_string(mpi_rank) +
              ": ");
        }

      MPI_Barrier(mpi_comm);
    }

  tic();
#pragma omp parallel for shared(result_local)
  for (unsigned int i = 0; i < n_rows_local; ++i)
    {
      for (unsigned int j = 0; j < n_cols; ++j)
        {
          result_local[i] += matrix_local[i * n_cols + j] * rhs[j];
        }
    }

  MPI_Barrier(mpi_comm);
  for (int rank = 0; rank < mpi_size; ++rank)
    {
      if (rank == mpi_rank && mpi_rank == 0)
        std::cout << std::endl;
      MPI_Barrier(mpi_comm);

      if (rank == mpi_rank)
        {
          toc("Matrix-vector product: time elapsed on rank " +
              std::to_string(mpi_rank) + ": ");
        }

      MPI_Barrier(mpi_comm);
    }

  tic();

  MPI_Gatherv(result_local.data(),
              result_local.size(),
              MPI_DOUBLE,
              result.data(),
              recv_counts.data(),
              recv_start_idx.data(),
              MPI_DOUBLE,
              0,
              mpi_comm);

  if (print && mpi_rank == 0)
    {
      std::cout << std::endl << "Result:" << std::endl;

      for (const auto &v : result)
        std::cout << std::setw(12) << v;

      std::cout << std::endl << std::endl;
    }

  MPI_Barrier(mpi_comm);
  for (int rank = 0; rank < mpi_size; ++rank)
    {
      if (rank == mpi_rank && mpi_rank == 0)
        std::cout << std::endl;
      MPI_Barrier(mpi_comm);

      if (rank == mpi_rank)
        {
          toc("Gather: time elapsed on rank " + std::to_string(mpi_rank) +
              ": ");
        }

      MPI_Barrier(mpi_comm);
    }

  MPI_Finalize();

  return 0;
}
