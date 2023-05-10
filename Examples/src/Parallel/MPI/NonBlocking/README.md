# An example of deadlock and of non-blocking communications #

in `main_deadlock.cpp` you have a classical example of deadlock. This is a prototype code meant to run with just 2 processes that communicate a message each other. The problem here is that both procsses launch a blocking `MPI_Recv` before sending! So both will wait for a message that cannot be received.

Try by launch the deadlock producing program with 

	mpirun -n 2 ./main_deadlock
	
You'll need to type `ctrl-C` (maybe more than once!) to stop the program.

The code `main_nodeadlock.cpp` presents the classical response for this situation: use `MPI_Sendrecv` to couple sending and receiving operations. This way, none of the two have precedence over the other. This is the best solution whenever you have 'cyclic' point-to-point communications, where each process has to send, and receive, a message from another process.
Try the code with 

	mpirun -n 2 ./main_nodeadlock
	
An alternative could be using non-blocking communications: i.e. point-to-point communication functions that return immediately the control to the program, without waiting the communication to complete. Non-blocking communications are in MPI indicated by `MPI_Iname`, where `MPI_Name` is the name of the generic counterpart. An example is in `main_noblocking`, where we also show the use of `MPI_Test` and `MPI_Wait`.

	mpirun -n 2 ./main_noblicking
	
to run it (again, this is a simple code that works only with just 2 processes).


# What do I learn here?

- An example of the dreaded deadlock, and how to avoid it.
- Non-blocking vs blocking.

## A note
In fact `MPI_Send` and `MPI_Recv` are not guaranteed to be blocking, 
**but is is safer to assume they are**. 
What they guarantee is that when they return the send/receive buffer can be safely reused.

Indeed, quoting the documentation of `MPI_Send` in [RookieHPC](https://rookiehpc.github.io/mpi/docs/mpi_send/index.html):

<em>Behind the scenes, MPI_Send will issue a buffered (asyncronous) send MPI_Bsend or a synchronous send MPI_Ssend. This decision will be based on whether the buffer attached for buffered sends contains enough free space for the message to send. If there is enough space, the buffered send MPI_Bsend will be issued, otherwise it will revert to a synchronous send MPI_Ssend. MPI implementations may provide a buffer by default, therefore not having explicitly assigned a buffer for buffered send does not guarantee that an MPI_Send will issue an MPI_Ssend. Tip: this specificity of MPI_Send to implicitly issue a buffered send or a synchronous send can result in deadlocks that are challenging to find out for beginners. 
Other variants of MPI_Send are MPI_Ssend, MPI_Bsend and MPI_Rsend. See MPI_Isend for the non-blocking counterpart of MPI_Send.<em>




