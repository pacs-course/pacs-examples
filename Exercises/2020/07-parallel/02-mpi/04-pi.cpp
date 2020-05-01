#include <mpi.h>
#include <omp.h>

#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>

static clock_t c_start, c_diff;
static double  c_sec;
#define tic() c_start = clock();
#define toc(x)                                      \
  c_diff = clock() - c_start;                       \
  c_sec  = (double)c_diff / (double)CLOCKS_PER_SEC; \
  std::cout << x << c_sec << " [s]" << std::endl;

/**
 * This exercise presents a simple program to determine the value of
 * pi. The algorithm suggested here is chosen for its simplicity. The
 * method evaluates the integral of 4 / (1 + x^2) between 0 and 1
 * using the composite midpoint rule. Each process adds up every
 * n-th interval. Finally, the sums computed by each process are
 * added together using a reduction.
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
              << ", number of threads: " << omp_get_num_threads()
              << std::endl;

  tic();

  const unsigned int n = 1e8;
  const double       h = 1.0 / n;

  double sum = 0.0;

#pragma omp parallel for reduction(+ : sum)
  for (unsigned int i = mpi_rank + 1; i <= n; i += mpi_size)
    {
      double x = h * (i - 0.5);
      sum += 4.0 / (1.0 + x * x);
    }

  double pi_local = h * sum;

  double pi;
  MPI_Reduce(&pi_local, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, mpi_comm);

  if (mpi_rank == 0)
    {
      std::cout << std::setprecision(16) << "pi = " << pi
                << ", error = "
                << std::fabs(pi - 3.141592653589793238462643)
                << std::endl;
    }

  // Trick to get output sorted by rank id.
  MPI_Barrier(mpi_comm);
  for (int rank = 0; rank < mpi_size; ++rank)
    {
      if (mpi_rank == rank)
        {
          toc("Time elapsed on rank " + std::to_string(rank) + ": ");
        }

      MPI_Barrier(mpi_comm);
    }

  MPI_Finalize();

  return 0;
}
