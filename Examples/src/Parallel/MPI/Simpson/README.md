# Parallel Composite Simpson Rule with MPI

This folder contains a simple MPI implementation of composite Simpson
integration.

The interval is split among the available MPI processes, each process computes
its local contribution, and the partial integrals are combined with
`MPI_Reduce`.

This is a good example of an almost embarrassingly parallel numerical task.

## Additional Features

The code also illustrates a few useful C++ techniques:

- reading simple input data from a JSON file
- storing the integrand in a `std::function`
- packing heterogeneous values into a `std::tuple`
- broadcasting the tuple as raw bytes

The JSON file provides the interval bounds and the number of subintervals.

## Running

After compilation, a typical execution is:

```bash
mpirun -n 2 ./main_simpsonMPI
```

## What You Learn Here

- how to parallelize a composite quadrature rule with MPI
- how to use `MPI_Reduce()` to sum local contributions
- a small example of JSON-based input
- a practical use of `std::tuple` for packaging data
