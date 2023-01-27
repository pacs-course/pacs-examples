#include <mpi.h>

#include <iostream>

/**
 * 
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

  if(mpi_rank == 0)
  {
    std::cout << "Running with  "<<mpi_size<<" processes."
              << std::endl;
  }

  // All process make a stop here
  MPI_Barrier(mpi_comm);

  const unsigned int num_iterations = 4;

// partner to which I send the message
  int partner_send = (mpi_rank+1)%mpi_size; // 0 --> 1, 1 --> 2.. p->0
// partner from which I receive the message
// I avoid mpi_rank-1 since maybe I have defined mpi_rank as unsigned!
// It is not this case, but here I show how to avoid a possible nasty
// mistake
  int partner_receive = (mpi_rank+mpi_size-1)%mpi_size; // 1 --> 0, 2 --> 1.. p->0

  // The MPI_Status struct contains the following members:
  // MPI_SOURCE, MPI_TAG, MPI_ERROR.
  MPI_Status status;
  int receive_data;
  int send_data;// the data I am sending
  // I use immediate return send to avoid
  // deadlock. Alternative (more elegant): use
  // MPI_Sendrecv to couple sends and receives

  MPI_Request request; // to verify when sending has ended
  for(unsigned int i = 0; i < num_iterations; ++i)
    {
      send_data=100*i+mpi_rank;
      MPI_Isend(&send_data, 1, MPI_INT, partner_send, 0, mpi_comm,&request);
      std::cout << "Iteration ="<<i<<" Rank " << mpi_rank << " ---> rank "
                    << partner_send << " data = " << send_data
                    << "." << std::endl;      
      MPI_Recv(&receive_data, 1, MPI_INT, partner_receive, 0, mpi_comm,
               &status);
      // The solution with MPI_Sendrecv
      //MPI_Sendrecv(&send_data, 1, MPI_INT,partner_send,0,
      //             &receive_data, 1, MPI_INT, partner_receive,0,mpi_comm,&status);

      std::cout << "Iteration ="<<i<<" Rank " << mpi_rank << " <--- rank "
                    << status.MPI_SOURCE << " data = " << receive_data
                    << "." << std::endl;
      // make sure send has been completed
      MPI_Wait(&request, MPI_STATUS_IGNORE);

    }

  MPI_Finalize();

  return 0;
}
