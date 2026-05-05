# Parallel Algorithms in the Standard Library

This folder contains a small example showing how to use the parallel algorithms
introduced in modern C++ through execution policies.

The key idea is that some standard algorithms accept an execution-policy object
 as their first argument. Typical policies are:

- `std::execution::seq`
  Sequential execution.

- `std::execution::par`
  Parallel execution using multiple threads.

- `std::execution::unseq`
  Vectorized or SIMD-style execution.

- `std::execution::par_unseq`
  A combination of parallel and vectorized execution when supported by the
  implementation.

The example in [`main.cpp`](./main.cpp) compares a few sequential and parallel
algorithms and measures their execution time.

## Notes

- These algorithms usually operate on ranges, just like the rest of the
  standard library.
- Actual performance depends heavily on the compiler, the standard library, and
  the hardware.
- On many systems, parallel standard-library algorithms require additional
  support libraries such as TBB.

## What You Learn Here

- how to call standard parallel algorithms
- what execution policies are
- how little code is required to experiment with parallelism in modern C++
