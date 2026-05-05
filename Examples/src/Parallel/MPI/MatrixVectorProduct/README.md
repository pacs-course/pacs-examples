# Parallel Matrix-Vector Product with MPI

Matrix-vector multiplication is one of the classic examples used to introduce
distributed linear algebra.

In this folder, the matrix is partitioned by rows across MPI processes, while
the vector is replicated where needed. The example also measures the time spent
in the different stages of the computation.

The code shows the use of several collective operations, including:

- broadcast
- scatter
- gather
- barriers for synchronization

It also highlights a practical issue: scattering the matrix can be expensive,
and storing the full matrix on the root process is memory-intensive. In real
applications one often tries to build local matrix blocks directly on each
process instead of assembling everything on rank 0 first.

The implementation also serves as a simple example of a hybrid design, since
each MPI process may in turn use OpenMP for local work.

## What You Learn Here

- a classic distributed matrix-vector product pattern
- the use of collective MPI communication
- the cost of setup and data movement in distributed linear algebra
