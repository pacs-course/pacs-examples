# Computing Pi with OpenMP

These programs approximate `pi` by integrating

\[
4 / (1 + x^2)
\]

over the interval `(0,1)` with the composite midpoint rule.

The folder contains two OpenMP implementations.

## `main_pi.cpp`

This version uses:

- a parallel region
- a `for` loop inside the parallel region
- a thread-local partial sum created with `firstprivate`
- an `atomic` update to accumulate the final result

It is instructive because it shows explicitly how local contributions are
combined.

## `main_pi_reduction.cpp`

This version uses the OpenMP `reduction` clause. It is shorter, clearer, and in
general a better solution for this type of accumulation.

## Running the Programs

After compilation, the executables can be run as:

```bash
./main_pi -t num_threads -e number_of_elements
./main_pi_reduction -t num_threads -e number_of_elements
```

If the options are omitted, default values are used.

## Numerical Note

The result may vary slightly when the number of threads changes. This is
expected: floating-point addition is not associative, so changing the order of
the operations changes the round-off error.

## What You Learn Here

- the use of `firstprivate`
- the use of `atomic`
- the use of `reduction`
- command-line parsing with `GetPot`
- why floating-point parallel reductions are not bitwise reproducible
