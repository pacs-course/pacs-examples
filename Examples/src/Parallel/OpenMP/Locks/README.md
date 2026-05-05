# OpenMP Locks

This example shows a typical use of OpenMP locks.

Two small classes are compared:

- one updates an internal counter without protection
- one protects the update with an OpenMP lock

The program then performs parallel updates on a vector of such objects, using a
list of repeated indices. When the updates are not protected, the final counts
may be wrong because several threads can modify the same object at the same
time. With locks, the result becomes correct and deterministic.

The example is intentionally simple: the goal is not performance, but showing
how a lock can make access to a shared object mutually exclusive.

## What You Learn Here

- a basic use of `omp_set_lock()` and `omp_unset_lock()`
- when a lock is needed
- the difference between protected and unprotected updates to shared state
