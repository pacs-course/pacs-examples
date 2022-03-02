#include <mpi.h>
#include <iostream>
#include <vector>

int
main (int argc, char *argv[])
{
  MPI_Init (&argc, &argv);

  int rank, size;
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Comm_size (MPI_COMM_WORLD, &size);

  int sendbuff = rank;
  std::vector<int> recvbuff (size, rank);

  // This is VERY dangerous!
  if (0) {
    for (int irank = 0; irank < size; ++irank) {
      if (irank == rank) continue;
      int stag = irank + size * rank;
      MPI_Send (&sendbuff, 1, MPI_INT, irank, stag, MPI_COMM_WORLD);
      std::cout << "rank " << rank << " sending message " << sendbuff << " to rank " << irank << std::endl;
      int rtag = rank + size * irank;
      MPI_Recv (&(recvbuff[irank]), 1, MPI_INT, irank, rtag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
  }

  // This is more debendable!
  std::vector<MPI_Request> reqs ((size - 1) * 2);
  int numreq = 0;
  if (0) {
    for (int irank = 0; irank < size; ++irank) {
      if (irank == rank) continue;
      int stag = irank + size * rank;
      MPI_Isend (&sendbuff, 1, MPI_INT, irank, stag, MPI_COMM_WORLD, &(reqs[numreq++]));
      std::cout << "rank " << rank << " sending message " << sendbuff << " to rank " << irank << std::endl;
      int rtag = rank + size * irank;
      MPI_Irecv (&(recvbuff[irank]), 1, MPI_INT, irank, rtag, MPI_COMM_WORLD, &(reqs[numreq++]));
    }
    MPI_Waitall (numreq, &(reqs[0]), MPI_STATUSES_IGNORE);
  }

  // MPI_Alltoall is much simpler!
  if (0) {
    //
  }

   // MPI_Allgather is much simpler!
  if (1) {
    //
  }
  
  for (int irank = 0; irank < size; ++irank) {
    if (rank == irank) {
      std::cout << " rank " << rank << " received messages ";
      for (auto ii : recvbuff )
        std::cout << ii << " ";
      std::cout << std::endl;
    }
    MPI_Barrier (MPI_COMM_WORLD);
  }
  
  MPI_Finalize ();
  return 0;
}
