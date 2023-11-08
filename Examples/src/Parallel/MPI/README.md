# A set of examples exploiting distributed memory parallelism using MPI

The `mpirun` (or `mpiexec`) command should be used to run an executable in parallel:
```bash
mpirun -n 4 ./executable1 # Runs on 4 processes.
mpirun -n 1 ./executable2 # Runs in serial.
./executable2             # Runs in serial.
```

* *HelloWorld* A simple examples showing the use of the basics of `MPI`, such as defining a communicator and computing the local process rank and the total number of processes.

* *Blocking* An example showing the use of point-to-point non-blocking communication through `MPI_Send()` and `MPI_Recv()`.

* *Probe* An example of probing an incoming message, *i.e.* checking whether the communication has completed succesfully and computing the size of the message.

* *NonBlocking* An example showing a possible issue of parallel programs, *i.e.* deadlock: since `MPI_Recv()` is a blocking operation, if two processes wait to receive a message at the same time the communication will never end. The use of the non-blocking `MPI_IRecv()` solves the problem and enables the communication to succeed.

* *PI* The analogous of the examples found in the `OpenMP` folder, here solved with hybrid parallelism (*i.e.* both using `OpenMP` and `MPI`).

* *MatrixVectorProduct* An example showing the use of collective operations, such as `MPI_Bcast()`, `MPI_Scatterv()` and `MPI_Gatherv()`, for computing a matrix-vector product in parallel.
* *PMatrix* An example of parallel matrix
* *OneAtATime* An example showing the use of `MPI_Barrier()` to synchronize the processes.
* *SendRecv* An example showing the use of `MPI_Sendrecv()` to perform a send and a receive operation in a single call.
* *Simpson* An example showing the use of `MPI_Reduce()` to compute an integral in parallel.
* *VectorSplit* An example showing the partitioning of a vector among MPI processes and the use of `MPI_Scatterv()` and `MPI_Gatherv()` to distribute and collect the vector elements.


