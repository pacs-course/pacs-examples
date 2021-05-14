#include <mpi.h>

#include <algorithm>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

/**
 * The length of a passed array, if not known a priori, can also be determined
 * using the MPI_Probe function. MPI_Probe can query a message without actually
 * receiving it. MPI_Probe will block, waiting for a message with a given tag
 * and sender, then retrieve the status of the message. Then MPI_Recv can be
 * used to retrieve the message using an array of the length calculated using
 * the status.
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

  if (mpi_rank == 0)
    {
      const int max_length = 100;

      std::random_device engine;

      // Allocate the array to be sent and fill it randomly.
      std::vector<double> array(max_length);

      std::uniform_real_distribution<double> rand(0, 1);
      std::generate(array.begin(), array.end(), [&engine, &rand]() {
        return rand(engine);
      });

      // Pick a random number to decide how long our message will be.
      std::uniform_int_distribution<int> randi(1, max_length);

      const int array_length = randi(engine);

      MPI_Send(array.data(), array_length, MPI_DOUBLE, 1, 10, mpi_comm);
      std::cout << "Process 0 sent " << array_length << " numbers to process 1."
                << std::endl
                << "    Last number sent: " << array[array_length - 1] << "."
                << std::endl
                << std::endl;
    }
  else // if (mpi_rank == 1)
    {
      MPI_Status status;

      // Probe for incoming message and get the status.
      MPI_Probe(0, 10, mpi_comm, &status);

      // Get the count from the status.
      int array_length;
      MPI_Get_count(&status, MPI_DOUBLE, &array_length);

      // Allocate some memory to put the message buffer into.
      // The result is converted back again to an std::vector.
      std::vector<double> array(array_length);
      MPI_Recv(array.data(),
               array_length,
               MPI_DOUBLE,
               MPI_ANY_SOURCE, // Or, more properly, status.MPI_SOURCE.
               MPI_ANY_TAG,    // Or, more properly, status.MPI_TAG.
               mpi_comm,
               MPI_STATUS_IGNORE);

      std::cout << "Process 1 received " << array_length << " numbers."
                << std::endl
                << "    Last number received: " << array.back() << "."
                << std::endl
                << "    Message source: rank " << status.MPI_SOURCE << "."
                << std::endl
                << "    Message tag:    " << status.MPI_TAG << "." << std::endl
                << std::endl;
    }

  MPI_Finalize();

  return 0;
}
