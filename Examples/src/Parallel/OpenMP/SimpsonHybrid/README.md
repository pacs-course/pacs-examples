# Hybrid MPI/OpenMP Simpson Integration

This folder contains a hybrid parallel version of the Simpson integration
example from `Parallel/MPI/Simpson`.

The general idea is the same:

- the integration interval is split across MPI processes
- each process computes a local composite Simpson rule
- the partial integrals are combined with `MPI_Reduce`

The difference is that the local Simpson computation is itself parallelized with
OpenMP using a `parallel for` and a reduction clause.

The input data are read from a JSON file, and the number of threads is also
configured there.

## Running

After compilation, a typical execution is:

```bash
mpirun -n 2 ./main_simpsonHybrid
```

## What You Learn Here

- a simple hybrid MPI/OpenMP design
- the use of `omp parallel for` with reduction
- the use of `MPI_Reduce()` to combine process-local results
- a small example of JSON-based configuration
