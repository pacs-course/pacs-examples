# Deadlock and Non-Blocking Communication

This folder contains three small MPI examples centered on deadlock.

## `main_deadlock.cpp`

This is an intentionally wrong program. Two processes both try to receive
before sending, so both wait forever.

Run it with:

```bash
mpirun -n 2 ./main_deadlock
```

and stop it manually.

## `main_nodeadlock.cpp`

This version fixes the issue using `MPI_Sendrecv`, which couples sending and
receiving in a single operation and is often the simplest solution for cyclic
exchanges.

## `main_non_blocking.cpp`

This version shows the non-blocking alternative, using calls such as:

- `MPI_Isend`
- `MPI_Irecv`
- `MPI_Test`
- `MPI_Wait`

## Important Note

For teaching purposes, it is safer to think of `MPI_Send` and `MPI_Recv` as
potentially blocking operations. In practice, the exact behavior may depend on
the MPI implementation and message buffering.

## What You Learn Here

- what a simple MPI deadlock looks like
- how `MPI_Sendrecv` can remove it
- when non-blocking communication is useful
