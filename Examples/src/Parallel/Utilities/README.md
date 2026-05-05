# Utilities for Parallel Examples

This folder contains helper utilities used by several examples in the `Parallel`
directory.

## `partitioner.hpp`

When a vector or a matrix has to be distributed across several processes or
threads, one first needs a partitioning strategy.

The utilities in `partitioner.hpp` help answer questions such as:

- where the `i`-th chunk starts and ends
- how many elements belong to each chunk
- which chunk contains a given global index
- how to build the `counts` and `displacements` arrays required by
  `MPI_Scatterv` and `MPI_Gatherv`

The file provides different partitioning strategies and also includes a
`MatrixPartitioner` helper for full matrices.

## `mpi_utils.hpp`

This file contains MPI-related helpers derived from the Muster code developed at
Lawrence Livermore National Laboratory. In particular, it defines MPI datatypes
that are convenient when sending C++ types such as `std::size_t`.

## `SafeMPI.hpp`

Including `mpi.h` can trigger noisy warnings with the course compilation flags,
especially when `-Wsuggest-override` is enabled. `SafeMPI.hpp` wraps the MPI
header and suppresses those warnings locally, so the rest of the code can still
be compiled with strict diagnostics.

## Build and Installation

If you simply run:

```bash
make
```

the folder builds a small test program for the partitioning utilities.

If you run:

```bash
make install
```

the headers are copied to the common `include` directory of the examples and
become available to other folders.

## What You Learn Here

- how balanced data partitioning is typically implemented
- how to prepare data layouts for MPI gather/scatter operations
- how to package MPI-related helper code for reuse
