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

  try
    {
      if (mpi_size != 2)
        {
          throw std::runtime_error("ERROR: This program is only "
                                   "meant to be run on 2 processes.");
        }
    }
  catch (const std::runtime_error &exc)
    {
      if (mpi_rank == 0)
        std::cerr << exc.what() << std::endl;

      return 1;
    }

  unsigned int num_iterations = 10;

  int ping_pong_count = 0;

  int        partner_rank = (mpi_rank + 1) % 2;
  MPI_Status status;

  for (unsigned int i = 0; i < num_iterations; ++i)
    {
      if (mpi_rank == ping_pong_count % 2)
        {
          ++ping_pong_count;
          MPI_Send(
            &ping_pong_count, 1, MPI_INT, partner_rank, 0, mpi_comm);

          std::cout << "Ping! Rank " << mpi_rank
                    << " sent     to   rank " << partner_rank
                    << " (ping_pong_count = " << ping_pong_count
                    << ")." << std::endl;
        }
      else
        {
          MPI_Recv(&ping_pong_count,
                   1,
                   MPI_INT,
                   partner_rank,
                   0,
                   mpi_comm,
                   &status);

          std::cout << "Pong! Rank " << mpi_rank
                    << " received from rank " << partner_rank
                    << " (ping_pong_count = " << ping_pong_count
                    << ")." << std::endl;
        }
    }

  MPI_Finalize();

  return 0;
}
