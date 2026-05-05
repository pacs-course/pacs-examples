# One Process at a Time

MPI does not have a direct equivalent of an OpenMP-style critical section for
arbitrary code blocks. This example shows a simple trick to emulate a
"one-process-at-a-time" region.

The idea is:

- all processes enter a loop over the ranks
- only the process whose rank matches the current loop value executes the block
- barriers are used to keep the ordering clean

This is especially useful when several processes must print diagnostic
information without interleaving their output.

## What You Learn Here

- how to serialize output in an MPI program
- a practical use of `MPI_Barrier()`
