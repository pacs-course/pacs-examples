# An example of vector partitioning in MPI #

This example shows how to partition a vector among MPI processes. The vector is first created on the root process, then partitioned and distributed to the other processes. The partitioning is done with the `MPI_Scatterv` function, which is the inverse of `MPI_Gatherv`.

Then, some statistics on the vector elements are computed in parallel. The results are collected on the root process with the `MPI_Reduce` function.

## What do I learn here? ##
* How to partition a vector among MPI processes
* How to use `MPI_Scatterv` and `MPI_Gatherv`
* How to use `MPI_Reduce` to collect results from different processes
