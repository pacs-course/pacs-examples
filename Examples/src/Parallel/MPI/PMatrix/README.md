# A Simple Parallel Matrix Class

This folder contains a class template representing a distributed full matrix for
MPI-based matrix-vector products.

The class supports two partitioning strategies:

- block-row partitioning
- block-column partitioning

The choice is driven automatically by the storage layout of the underlying local
matrix type.

## Main Idea

The `PMatrix` class receives a global matrix on the master process, partitions
it, and distributes the local pieces across the MPI communicator. Once the
distribution is complete, the class can perform parallel matrix-vector products.

The result can then be:

- collected only on the root process
- collected on all processes

depending on which gathering method is used.

## Why This Example Matters

The example is not only about matrix-vector multiplication. It also shows an
important performance lesson: distributing the matrix from one root process can
be much more expensive than the local computation itself. This is often the
dominant setup cost in toy implementations.

In practice, this kind of class is most useful when:

- the same matrix is reused many times
- the setup cost can be amortized

## Build Note

This example depends on utilities installed from:

- `Parallel/Utilities`
- `Matrix`

If compilation fails because headers are missing, build and install those
dependencies first.

## What You Learn Here

- how a simple distributed matrix abstraction can be designed
- the difference between row-block and column-block partitioning
- why data distribution cost matters in parallel linear algebra
