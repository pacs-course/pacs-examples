# Fibonacci Numbers with OpenMP Tasks

This folder compares several ways of computing Fibonacci numbers:

- a recursive serial version
- a non-recursive serial version
- a recursive task-based OpenMP version

The OpenMP implementation uses tasks because the recursion tree does not map
naturally to a simple `parallel for`.

The parallel idea is straightforward: once `F(n)` is split into `F(n-1)` and
`F(n-2)`, those two recursive calls can in principle be computed concurrently.
The program therefore creates tasks for the two branches and synchronizes them
with `taskwait`.

## Why This Example Is Interesting

This is a good example of a parallelization that is technically possible but
often not worthwhile.

The reason is that:

- each task performs very little work
- the number of tasks grows very quickly
- task creation and synchronization overhead can dominate the useful work

The non-recursive serial implementation is usually much faster on ordinary
machines.

## Running the Program

```bash
./main_fibs -t num_threads -l task_limit
```

- `-t` sets the number of threads
- `-l` sets the threshold below which the recursive code switches back to the
  serial version

The program is interactive and asks which Fibonacci number should be computed.

## What You Learn Here

- how OpenMP tasks are used
- why recursive algorithms are not always good candidates for parallelism
- why algorithmic structure matters more than simply adding threads
