# Parallel Computing Examples

This directory collects examples of parallel programming based on four main
approaches:

- `ParallelSTL`
  Examples using the parallel algorithms of the C++ standard library.

- `OpenMP`
  Shared-memory parallel programming on a single machine.

- `MPI`
  Distributed-memory parallel programming with message passing.

- `Utilities`
  Helper classes and functions used by some of the other examples.

The examples are intentionally small and focused. Some are designed to show a
correct parallel pattern, while others illustrate typical problems such as data
races or deadlocks.
