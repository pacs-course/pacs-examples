#include <mpi.h>

#include <iostream>
#include <vector>

/**
 * Solution to deadlock of 04-deadlock.cpp using non-blocking communication.
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

  const std::vector<double> to_send(100, mpi_rank);

  std::vector<double> to_receive(100);

  const int tag_send    = (mpi_rank == 0) ? 10 : 20;
  const int tag_receive = (mpi_rank == 0) ? 20 : 10;

  const int partner_rank = !mpi_rank; // 0 --> 1, 1 --> 0.

  std::vector<MPI_Request> requests(mpi_size);
  std::vector<MPI_Status>  statuses(mpi_size);
  int                      ready;

  MPI_Irecv(to_receive.data(),
            to_receive.size(),
            MPI_DOUBLE,
            partner_rank,
            tag_receive,
            mpi_comm,
            &requests[0]);

  MPI_Isend(to_send.data(),
            to_send.size(),
            MPI_DOUBLE,
            partner_rank,
            tag_send,
            mpi_comm,
            &requests[1]);

  // Test for all requests to complete.
  MPI_Testall(mpi_size, requests.data(), &ready, MPI_STATUS_IGNORE);
  std::cout << "Test on rank " << mpi_rank << ": non-blocking communications "
            << (ready ? "" : "not yet ") << "completed." << std::endl;

  // Wait for all communications to finish.
  MPI_Waitall(mpi_size, requests.data(), statuses.data());

  // Test again: all requests complete after calling MPI_Waitall.
  MPI_Testall(mpi_size, requests.data(), &ready, MPI_STATUS_IGNORE);
  std::cout << "Wait on rank " << mpi_rank << ": non-blocking communications "
            << (ready ? "" : "not yet ") << "completed." << std::endl;

  // statuses[0] contains the outcome of MPI_Irecv.
  int recv_count;
  MPI_Get_count(&statuses[0], MPI_DOUBLE, &recv_count);

  std::cout << "Process " << mpi_rank << " received " << recv_count
            << " numbers." << std::endl
            << "    Message source: rank " << statuses[0].MPI_SOURCE << "."
            << std::endl
            << "    Message tag:    " << statuses[0].MPI_TAG << "." << std::endl
            << std::endl;

  MPI_Finalize();

  return 0;
}
