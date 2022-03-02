#include <mpi.h>

#include <iostream>

/**
 * Process 0 receives a number from and sends a number to process 1.
 * Process 1 receives a number from and sends a number to process 0.
 *
 * MPI_Recv is blocking, so the communication never ends.
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

  int number_to_receive = mpi_rank;
  int number_to_send    = 2 * mpi_rank;

  int partner_rank = !mpi_rank;

  MPI_Status status;

  if (mpi_rank == 0)
    {
      std::cout << "Rank " << mpi_rank << " waiting to receive..."
                << std::endl;

      MPI_Recv(&number_to_receive,
               1,
               MPI_INT,
               partner_rank,
               20,
               mpi_comm,
               &status);

      std::cout << "Rank " << mpi_rank << " received." << std::endl;

      MPI_Send(
        &number_to_send, 1, MPI_INT, partner_rank, 10, mpi_comm);

      std::cout << "Rank " << mpi_rank << " sent." << std::endl;
    }
  else // if (mpi_rank == 1)
    {
      std::cout << "Rank " << mpi_rank << " waiting to receive..."
                << std::endl;

      MPI_Recv(&number_to_receive,
               1,
               MPI_INT,
               partner_rank,
               10,
               mpi_comm,
               &status);

      std::cout << "Rank " << mpi_rank << " received." << std::endl;

      MPI_Send(
        &number_to_send, 1, MPI_INT, partner_rank, 20, mpi_comm);

      std::cout << "Rank " << mpi_rank << " sent." << std::endl;
    }

  MPI_Finalize();

  return 0;
}
