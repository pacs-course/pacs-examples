# OpenMP Scheduling Strategies

This folder contains a small program illustrating how OpenMP distributes loop
iterations among threads under different scheduling strategies.

The example focuses on the main policies used with:

```cpp
#pragma omp for schedule(kind, chunk_size)
```

## Strategies Shown

- `static`
  Iterations are assigned in a predictable way, usually in equally sized
  portions.

- `dynamic`
  Threads request more work as they finish their current chunk.

- `guided`
  Similar to `dynamic`, but chunk sizes decrease as the loop progresses.

- `runtime`
  The strategy is taken from `OMP_SCHEDULE` or from `omp_set_schedule()`.

The program prints which thread executes which iteration, so the distribution
can be inspected directly.

## Notes

- `static` scheduling tends to be best when all iterations have similar cost.
- `dynamic` and `guided` are useful when the workload is unbalanced.
- Different runs may produce different assignments for `dynamic` and `guided`.

## What You Learn Here

- the practical meaning of the main scheduling policies
- how chunk size affects work distribution
- how to experiment with `OMP_SCHEDULE` and `omp_set_schedule()`
