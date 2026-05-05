# A Simple Data Race Example

This folder contains a deliberately incorrect OpenMP example showing how a data
race can arise.

The serial loop is:

```cpp
for(size_t i = 0; i < n - 1; ++i)
  a[i] = a[i + 1] + b[i];
```

The same loop is then parallelized with `#pragma omp for`, but this is wrong:
each iteration depends on data that may be modified by another iteration.

As a consequence:

- the result becomes nondeterministic
- different runs may produce different answers
- changing the scheduling strategy does not fix the logic error

The example also prints the serial and parallel checksums so the difference is
easy to observe.

## What You Learn Here

- what a data race looks like in practice
- why not every loop can be parallelized safely
- how dependence between iterations must be analyzed before adding OpenMP
