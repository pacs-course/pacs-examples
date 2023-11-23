#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-override"
#pragma GCC diagnostic ignored "-Wcast-function-type"
#include <mpi.h>
#pragma GCC diagnostic pop

#include "GetPot"
#include <cmath>
#include <iomanip>
#include <iostream>

/*
 * We use here the native MPI tools for timings (with a little trick to create
 * tic and toc like in Matlab)
 */
static double c_start, c_diff;
#define tic() c_start = MPI_Wtime();
#define toc(x)                                       \
  {                                                  \
    c_diff = MPI_Wtime() - c_start;                  \
    std::cout << x << c_diff << " [s]" << std::endl; \
  }

void
printHelp()
{
  std::cout << "To run the program\n";
  std::cout << "mpirun -n num_proc ./main_pi -n <number_of_elemts>";
  std::cout
    << "num_proc: the number of processes used by MPI,\n"
    << "number_of_elemts: The number of elements for the integration, use a "
       "big number. default: 1e9)\n"
    << std::endl;
}
/**
 * This exercise presents a simple program to determine the value of pi. The
 * algorithm suggested here is chosen for its simplicity. The method evaluates
 * the integral of 4 / (1 + x^2) over the interval (0, 1) using the composite
 * midpoint rule. Each process adds up every p-th interval (where p is the
 * number of processes. Finally, the sums computed by each process are added
 * together using a reduction.
 *
 * This example makes use of  MPI.
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
  unsigned int n;
  bool         stop{false};
  if(mpi_rank == 0)
    {
      GetPot gp(argc, argv);
      if(gp.search(2, "-h", "--help"))
        {
          printHelp();
          stop = true;
          //@todo broadcast stop to all processes
        }
      n = gp.follow(1000000000u, "-n");
      std::cout << "Number of Intervals: " << n << " ";
      std::cout << "Number of processes: " << mpi_size
                << std::endl;
    }

  if(stop)
    {
      MPI_Finalize();
      //MPI_Abort(mpi_comm, 0);
      return 0;
    }

  tic();
  MPI_Bcast(&n, 1, MPI_UNSIGNED, 0, mpi_comm);
  const double h = 1.0 / n;

  double sum = 0.0;

  for(unsigned int i = mpi_rank; i < n; i += mpi_size)
    {
      const double x = h * (i + 0.5);
      sum += 4.0 / (1.0 + x * x);
    }

  double pi = h * sum;

  // Reduction necessary to get the final result.
  // MPI_IN_PLACE specifies that the input and output buffer are the same!
  // If you want the local pi and global pi computation be kept in different places
  // see the comment below

  MPI_Allreduce(MPI_IN_PLACE, &pi, 1, MPI_DOUBLE, MPI_SUM, mpi_comm);

  // The previous lines could be replaced by, e.g.:
  // const double pi_local = h * sum;
  // double pi;
  // MPI_Reduce(&pi_local, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, mpi_comm);

  if(mpi_rank == 0)
    {
      std::cout << std::setprecision(16) << "pi = " << pi
                << ", error = " << std::abs(pi - M_PI) << std::endl;
    }

  // Trick to get output sorted by rank id.
  MPI_Barrier(mpi_comm);
  int rank=0;
  while (rank < mpi_size)
    {
      if(mpi_rank == rank)
        {
          toc("Time elapsed on rank " + std::to_string(mpi_rank) + ": ");
        }
      ++rank;
      MPI_Barrier(mpi_comm);
    }

  MPI_Finalize();

  return 0;
}
