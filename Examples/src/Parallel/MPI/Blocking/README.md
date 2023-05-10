#A first example of point-to-point communication #

It works only when launched with two processes with `mpirun -n 2 main_ping_pong`.

The two processes send messages each other by the use of the basic MPI mechanism for point-to-point
communication.

# What do I learn here? #

A basic usage of `MPI_Send` and `MPI_Recv`
