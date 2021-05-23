#include <mpi.h>
#include <omp.h>

#include <cmath>
#include <iomanip>
#include <iostream>

static double c_start, c_diff;
#define tic() c_start = MPI_Wtime();
#define toc(x)                                       \
  {                                                  \
    c_diff = MPI_Wtime() - c_start;                  \
    std::cout << x << c_diff << " [s]" << std::endl; \
  }

/**
 * This exercise presents a simple program to determine the value of pi. The
 * algorithm suggested here is chosen for its simplicity. The method evaluates
 * the integral of 4 / (1 + x^2) over the interval (0, 1) using the composite
 * midpoint rule. Each process adds up every p-th interval (where p is the
 * number of processes. Finally, the sums computed by each process are added
 * together using a reduction.
 *
 * This example makes use of hybrid shared/distributed parallelization
 * through OpenMP and MPI.
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

  tic();

  const unsigned int n = 1e9;
  const double       h = 1.0 / n;

  double sum = 0.0;

#pragma omp parallel for reduction(+ : sum)
  for (unsigned int i = mpi_rank; i < n; i += mpi_size)
    {
      const double x = h * (i + 0.5);
      sum += 4.0 / (1.0 + x * x);
    }

  double pi = h * sum;
  MPI_Allreduce(MPI_IN_PLACE, &pi, 1, MPI_DOUBLE, MPI_SUM, mpi_comm);

  // The previous lines could be replaced by, e.g.:
  // const double pi_local = h * sum;
  // double pi;
  // MPI_Reduce(&pi_local, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, mpi_comm);

  if (mpi_rank == 0)
    {
      std::cout << std::setprecision(16) << "pi = " << pi
                << ", error = " << std::abs(pi - M_PI) << std::endl;
    }

  // Trick to get output sorted by rank id.
  MPI_Barrier(mpi_comm);
  for (int rank = 0; rank < mpi_size; ++rank)
    {
      if (mpi_rank == rank)
        {
          toc("Time elapsed on rank " + std::to_string(mpi_rank) + ": ");
        }

      MPI_Barrier(mpi_comm);
    }

  MPI_Finalize();

  return 0;
}
