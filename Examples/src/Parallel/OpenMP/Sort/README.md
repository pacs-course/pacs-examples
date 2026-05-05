# Comparing Parallel Sorting Algorithms

This folder compares several sorting approaches on large vectors.

The example includes:

1. the standard serial `std::sort`
2. the native C++ parallel version of `std::sort`, when enabled
3. an OpenMP task-based quicksort
4. an experimental odd-even transposition sort, currently disabled because it
   is not competitive

The main focus is not sorting itself, but the comparison between different
parallelization strategies.

## OpenMP Quicksort

The OpenMP version is based on recursive partitioning. Once a pivot is chosen,
the two recursive branches can be launched as separate tasks. This makes the
example a good companion to the `Fibonacci` folder: both use recursion, but
quicksort is a much more suitable task-based parallel algorithm.

## Building

Two build modes are supported:

```bash
make all
make parallel_cpp
```

If you switch from one mode to the other, run `make clean` first.

## Running

```bash
./main_sort [-t p] [-h] [-size number_of_elements]
```

- `-t` sets the number of threads
- `-size` sets the vector size

## What You Learn Here

- how to use OpenMP tasks in a realistic recursive algorithm
- how to compare hand-written OpenMP code with library-provided parallel code
- why not every parallel sorting algorithm is equally scalable
