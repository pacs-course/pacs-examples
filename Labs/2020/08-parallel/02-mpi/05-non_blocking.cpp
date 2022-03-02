#include <mpi.h>

#include <iostream>
#include <vector>

/**
 * Solution to deadlock of 04-deadlock.cpp using non-blocking
 * communication.
 * The vector "to_receive" in rank 0 is replaced with
 * the vector "to_send" in rank 1, and vice-versa.
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

  std::vector<double> to_send(100, mpi_rank);
  std::vector<double> to_receive(100, mpi_rank);

  int tag_send    = (mpi_rank == 0) ? 10 : 20;
  int tag_receive = (mpi_rank == 0) ? 20 : 10;

  int partner_rank = !mpi_rank;

  MPI_Request request;
  MPI_Status  status;
  int         ready;

  MPI_Irecv(to_receive.data(),
            to_receive.size(),
            MPI_DOUBLE,
            partner_rank,
            tag_receive,
            mpi_comm,
            &request);

  MPI_Test(&request, &ready, MPI_STATUS_IGNORE);
  std::cout << "Test on rank " << mpi_rank
            << ": non-blocking communication "
            << (ready ? "" : "not yet ") << "completed." << std::endl;

  MPI_Send(to_send.data(),
           to_send.size(),
           MPI_DOUBLE,
           partner_rank,
           tag_send,
           mpi_comm);

  // Wait for the communication to end.
  MPI_Wait(&request, &status);

  std::cout << "Wait on rank " << mpi_rank
            << ": non-blocking communication completed." << std::endl
            << std::endl;

  int recv_count;
  MPI_Get_count(&status, MPI_DOUBLE, &recv_count);

  std::cout << "Process " << mpi_rank << " received " << recv_count
            << " numbers." << std::endl
            << "    Message source: rank " << status.MPI_SOURCE << "."
            << std::endl
            << "    Message tag:    " << status.MPI_TAG << "."
            << std::endl
            << std::endl;

  MPI_Finalize();

  return 0;
}
