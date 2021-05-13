#include <mpi.h>

#include <iostream>

int
main(int argc, char **argv)
{
  /**
   * Initialize MPI and define the communicator.
   * MPI_Init constructs all of MPI's global and internal variables.
   * For example, a communicator is formed around all of the processes
   * that were spawned and unique ranks are assigned to each process.
   * No MPI calls can be made before this call.
   */
  MPI_Init(&argc, &argv);
  MPI_Comm mpi_comm = MPI_COMM_WORLD;

  /**
   * Get the number of processes.
   * MPI_Comm_size returns the size of the communicator. In this
   * example, mpi_comm (which MPI constructs) encloses all of
   * the processes in the job, so this call should return the amount
   * of processes that were requested for the job on the command line.
   */
  int mpi_size;
  MPI_Comm_size(mpi_comm, &mpi_size);

  /**
   * Get the rank of current process.
   * MPI_Comm_rank returns the rank of a process in a communicator.
   * Each process inside of a communicator is assigned an incremental
   * rank starting from 0. The ranks of the processes are primarily
   * used for identification purposes when sending and receiving
   * messages.
   */
  int mpi_rank;
  MPI_Comm_rank(mpi_comm, &mpi_rank);

  /**
   * Get the name of the processor.
   * MPI_Get_processor_name obtains the actual name of the processor
   * this process is being run on.
   */
  char proc_name[MPI_MAX_PROCESSOR_NAME];
  int  name_length;
  MPI_Get_processor_name(proc_name, &name_length);

  std::cout << "Hello world! This is rank " << mpi_rank << " out of "
            << mpi_size << " processors, on " << proc_name << "." << std::endl;

  /**
   * Finalize MPI.
   * MPI_Finalize cleans up the MPI environment. No more MPI calls can
   * be made after this call.
   */
  MPI_Finalize();

  return 0;
}
