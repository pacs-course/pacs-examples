# Splitting a Vector Across MPI Processes

This example shows a typical MPI data-distribution pattern.

The root process computes how a global vector should be partitioned, then:

- chunk sizes are distributed with `MPI_Scatter`
- local vectors are created on each process
- each process works on its local data
- results are collected with `MPI_Gather`, `MPI_Gatherv`, and reductions

In the current implementation, each process fills its local chunk with random
numbers and computes local statistics such as:

- mean value
- minimum
- maximum

The root process then reconstructs the global results and compares them with the
statistics computed from the gathered full vector.

## What You Learn Here

- how to partition a vector among processes
- how to use `MPI_Scatter`, `MPI_Gather`, and `MPI_Gatherv`
- how to assemble global information from local computations
