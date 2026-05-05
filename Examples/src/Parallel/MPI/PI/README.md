# Computing Pi with MPI

This example approximates `pi` by integrating `4 / (1 + x^2)` over `(0,1)`
with a composite quadrature rule.

The main purpose of the code is to illustrate collective reduction, not to
provide the best possible numerical method.

The work is split across MPI processes, each process computes a partial sum, and
the final result is obtained with `MPI_Allreduce`.

## Running

```bash
mpirun -n num_procs ./main_pi [-h] [-n num_elements]
```

- `-h` prints a short help message
- `-n` sets the number of subintervals

The code also uses `GetPot` to parse the command-line options.

## Numerical Note

The result may change slightly when the number of processes changes, because the
floating-point operations are accumulated in a different order.

## What You Learn Here

- how to use `MPI_Allreduce`
- command-line parsing with `GetPot`
- timing with MPI utilities
- why parallel floating-point computations are not bitwise reproducible
