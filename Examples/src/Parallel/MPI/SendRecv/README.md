# Parallel Sorting with `MPI_Sendrecv`

This example implements an odd-even transposition sort distributed across MPI
processes.

The global vector is split into local chunks, each process sorts its own chunk,
and then neighboring processes repeatedly exchange data. After each exchange,
the lower-ranked process keeps the smaller values and the higher-ranked process
keeps the larger ones.

The communication pattern alternates between:

- even phases: pairs `(0,1)`, `(2,3)`, ...
- odd phases: pairs `(1,2)`, `(3,4)`, ...

This is a natural setting for `MPI_Sendrecv`, since each process sends and
receives in the same step.

## Running

```bash
mpirun -n p ./main_sendrecv [-h] [-size number_of_elements]
```

The input size is intentionally large because the example is mainly about timing
and communication patterns.

## Important Note

This algorithm is pedagogically useful, but it is not a very efficient parallel
sorting method. Its communication cost grows badly with the number of processes,
so on an ordinary machine it is often slower than the serial version.

## What You Learn Here

- how to use `MPI_Sendrecv`
- how a nearest-neighbor communication pattern can be organized
- why a correct parallel algorithm is not necessarily a scalable one
