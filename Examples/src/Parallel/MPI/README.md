# MPI Examples

This directory contains examples based on distributed-memory parallelism with
MPI.

MPI programs are normally launched with `mpirun` or `mpiexec`, for example:

```bash
mpirun -n 4 ./executable
```

## Subfolders

- `HelloWorld`
  The minimal structure of an MPI program.

- `Blocking`
  Basic point-to-point communication with `MPI_Send` and `MPI_Recv`.

- `Circular`
  Ring-style communication among all processes.

- `NonBlocking`
  Deadlock examples and the use of non-blocking communication.

- `OneAtATime`
  A simple trick to serialize output across ranks.

- `PI`
  Parallel numerical integration with reduction.

- `Probe`
  Receiving messages of unknown size with `MPI_Probe`.

- `VectorSplit`
  Distributing a vector across processes and collecting results.

- `MatrixVectorProduct`
  Parallel matrix-vector multiplication with collective communication.

- `PMatrix`
  A small parallel matrix class built on top of MPI.

- `SendRecv`
  Odd-even transposition sorting and the use of `MPI_Sendrecv`.

- `Simpson`
  Composite Simpson integration in parallel.
