# An example of "circular" point-to-point communication #

Every process sends a message to the process with rank+1 modulo the
number of processes.

To avoid deadlock, I have chosen here a asyncronous non-blocking
(immediate return) send.

You may note that, while messages are correctly sent/received, the order is not deterministic. This is quite common: every process run independently from the others.


# What do I learn here? #

A less basic usage of `MPI_Isend` and `MPI_Recv
`
The use of a `MPI_Request` and `MPI_Wait`. 

