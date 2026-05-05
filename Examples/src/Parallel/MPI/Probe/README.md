# Receiving Messages of Unknown Size

`MPI_Probe` lets a process inspect a pending message without actually receiving
it. This is useful when the message size is not known in advance.

The usual pattern is:

1. probe the message
2. inspect the resulting `MPI_Status`
3. use `MPI_Get_count` to determine how large the receive buffer should be
4. allocate the buffer
5. receive the message

That is exactly what this example demonstrates.

The program is meant to be run with two processes:

```bash
mpirun -n 2 ./main_probe
```

## What You Learn Here

- how `MPI_Probe` works
- how to use `MPI_Get_count`
- how to allocate receive buffers dynamically and safely
