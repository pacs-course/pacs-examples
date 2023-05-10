# A critical section in MPI #
A critical section is a block of code that can be executed by a process at a time. MPI doeas not have a specific function to define critical section. We have `MPI_Barrier()` that however is just a syncronization point: all processes stop until the last one calls it.

To simulate a critical section we have to use a little trick. 

```
  int rank=0;
  MPI_Barrier(MPI_COMM_WORLD);
  while (rank < mpi_size) // all processes carry the loop
     {
      if(mpi_rank==rank) //but only this one enters the block
      {
          // This is the critical section
      }
      rank++;
      MPI_Barrier(mpi_comm); //syncronize processes
     }
```

The last `MPI_Barrier()` is optional. Its role is to force the rank ordering in accessing the critical section. If you comment it out, ordering is not guaranteed. 

In the example in this folder, the trick is used to let each process to print some data on the screen without interfering each other.


# What do I learn here? #
A trick to implement a critical section in MPI