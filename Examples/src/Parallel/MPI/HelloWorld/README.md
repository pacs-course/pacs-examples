# MPI Hello World

This is the classic introductory MPI program.

Each process starts, discovers:

- its rank
- the total number of processes
- the communicator being used

and prints a message.

There is no communication among processes here. The example is only meant to
show the basic structure of an MPI program and the use of `MPI_Init`,
`MPI_Comm_rank`, `MPI_Comm_size`, and `MPI_Finalize`.

Different runs may print the messages in different orders. This is normal.
