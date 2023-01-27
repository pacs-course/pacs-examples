/*
 * main.cpp
 *
 *  Created on: Oct 21, 2022
 *      Author: forma
 */
#include <iostream>
#include <mpi.h>

int main(int argc, char** argv)
{
  int mpi_rank;
  int mpi_size;

  // Initilialise MPI and check its completion
  MPI_Init(&argc, &argv);
  MPI_Comm mpi_comm = MPI_COMM_WORLD;
  MPI_Comm_rank(mpi_comm, &mpi_rank);
  MPI_Comm_size(mpi_comm, &mpi_size);

  if(mpi_rank==0)
    std::cout<<"N of processes="<<mpi_size<<std::endl;

 // Prints one at a time (in an undetermined order)

  int rank=0;
  MPI_Barrier(MPI_COMM_WORLD);
  while (rank < mpi_size) // all processes carry the loop
     {
      if(mpi_rank==rank) //but only this one prints
      {
          // This is the critical section
        std::cout<<"Process rank="<< mpi_rank<<" is printing"<<std::endl;
      }
      rank++;
      MPI_Barrier(mpi_comm);
     }

  MPI_Finalize();
}



