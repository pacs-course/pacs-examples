#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-override"
#pragma GCC diagnostic ignored "-Wcast-function-type"
#include <mpi.h>
#pragma GCC diagnostic pop

#include <iostream>

/**
 * Process 0 receives a number from and sends a number to process 1.
 * Process 1 receives a number from and sends a number to process 0.
 *
 * MPI_Recv is blocking, so the communication never ends.
 *
 * As a possible solution, the two calls to MPI_Recv and to MPI_Send may be
 * swapped (at least on one rank), or non-blocking communications may be used
 * (see exercise 05).
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

  if(mpi_size != 2)
    {
      if(mpi_rank == 0)
        std::cerr << "ERROR: This program is only "
                     "meant to be run on 2 processes."
                  << std::endl;

      return 1;
    }

  const int number_to_send = mpi_rank;

  int number_to_receive;

  const int partner_rank = !mpi_rank; // 0 --> 1, 1 --> 0.


  if(mpi_rank == 0)
    {
      std::cout << "Rank " << mpi_rank << " waiting to receive..." << std::endl;
      // SendRecv is also blacking, but it couples a send and a receive, so it
      // may avoid dedlocks
      MPI_Sendrecv(&number_to_send, 1, MPI_INT, partner_rank, 10,
                   &number_to_receive, 1, MPI_INT, partner_rank, 20, mpi_comm,
                   MPI_STATUS_IGNORE);
      std::cout << "Rank " << mpi_rank << " received message "
                << number_to_receive << std::endl;
      std::cout << "Rank " << mpi_rank << " sent message     " << number_to_send
                << std::endl;
    }
  else // if (mpi_rank == 1)
    {
      std::cout << "Rank " << mpi_rank << " waiting to receive..." << std::endl;
      MPI_Sendrecv(&number_to_send, 1, MPI_INT, partner_rank, 20,
                   &number_to_receive, 1, MPI_INT, partner_rank, 10, mpi_comm,
                   MPI_STATUS_IGNORE);
      std::cout << "Rank " << mpi_rank << " received message "
                << number_to_receive << std::endl;
      std::cout << "Rank " << mpi_rank << " sent message     " << number_to_send
                << std::endl;
    }

  MPI_Finalize();

  return 0;
}
