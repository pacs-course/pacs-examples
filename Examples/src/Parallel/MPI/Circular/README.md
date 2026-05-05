# Circular MPI Communication

In this example, each process sends a message to the next process in a ring:

- rank `0` sends to rank `1`
- rank `1` sends to rank `2`
- ...
- the last rank sends back to rank `0`

The code uses a non-blocking send so that the circular communication can be
implemented safely without creating a deadlock.

Because the processes run independently, the order in which messages are printed
or completed is not deterministic.

## What You Learn Here

- how to implement ring-style communication
- the use of `MPI_Isend`
- the roles of `MPI_Request` and `MPI_Wait`
