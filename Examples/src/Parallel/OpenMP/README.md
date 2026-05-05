# OpenMP Examples

This directory contains examples of shared-memory parallel programming with
OpenMP.

The number of threads can usually be controlled through the environment
variable `OMP_NUM_THREADS`, for example:

```bash
export OMP_NUM_THREADS=4
./executable
```

or, for a single command only:

```bash
OMP_NUM_THREADS=4 ./executable
```

## Subfolders

- `HelloWorld`
  Basic OpenMP constructs such as `parallel`, `critical`, `barrier`, and
  thread identifiers.

- `DataRace`
  A deliberately wrong example showing how a data race can corrupt a result.

- `PI`
  Parallel numerical integration with both an explicit accumulation strategy and
  a reduction-based implementation.

- `Fibonacci`
  Recursive and task-based Fibonacci examples, mainly to discuss when task
  parallelism is a poor fit.

- `Locks`
  A simple use of OpenMP locks to protect updates to shared state.

- `Scheduling`
  A small program illustrating the behavior of the main OpenMP scheduling
  strategies.

- `Sort`
  Comparison of several sorting approaches, including OpenMP task-based
  quicksort.

- `SimpsonHybrid`
  A hybrid MPI/OpenMP version of the Simpson integration example.
