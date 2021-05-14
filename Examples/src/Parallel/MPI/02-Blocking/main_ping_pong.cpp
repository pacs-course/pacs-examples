#include <mpi.h>

#include <iostream>

/**
 * The classical Ping Pong MPI example.
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

  if (mpi_size != 2)
    {
      if (mpi_rank == 0)
        std::cerr << "ERROR: This program is only "
                     "meant to be run on 2 processes."
                  << std::endl;

      return 1;
    }

  const unsigned int num_iterations = 10;

  int ping_pong_count = 0;

  const int partner_rank = !mpi_rank; // 0 --> 1, 1 --> 0.

  // The MPI_Status struct contains the following members:
  // MPI_SOURCE, MPI_TAG, MPI_ERROR.
  MPI_Status status;

  for (unsigned int i = 0; i < num_iterations; ++i)
    {
      if (mpi_rank == ping_pong_count % 2)
        {
          ++ping_pong_count;

          MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0, mpi_comm);

          std::cout << "Ping! Rank " << mpi_rank << " ---> rank "
                    << partner_rank << " (count = " << ping_pong_count << ")."
                    << std::endl;
        }
      else
        {
          MPI_Recv(
            &ping_pong_count, 1, MPI_INT, partner_rank, 0, mpi_comm, &status);

          // "status.MPI_SOURCE" equals "partner_rank".
          std::cout << "Pong! Rank " << mpi_rank << " <--- rank "
                    << status.MPI_SOURCE << " (count = " << ping_pong_count
                    << ")." << std::endl;
        }
    }

  MPI_Finalize();

  return 0;
}
