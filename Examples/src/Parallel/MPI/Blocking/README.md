# Blocking Point-to-Point Communication

This folder contains a very small example of basic MPI point-to-point
communication.

The code is meant to be run with exactly two processes:

```bash
mpirun -n 2 ./main_ping_pong
```

The two ranks exchange messages using the classic blocking pair:

- `MPI_Send`
- `MPI_Recv`

It is a minimal "ping-pong" style example and is mainly useful as a first
introduction to direct communication between two processes.

## What You Learn Here

- the basic syntax of blocking point-to-point communication
- the difference between sender and receiver roles
