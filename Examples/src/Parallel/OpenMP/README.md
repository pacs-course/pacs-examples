# A set of examples exploiting shared memory parallelism using OpenMP

The number of parallel threads used to run an executable can be set through the `OMP_NUM_THREADS` environment variable, *e.g.* with
```bash
export OMP_NUM_THREADS=4
./executable1 # Runs on 4 threads.
./executable2 # Runs on 4 threads.
```
or, without permanently altering the environment:
```bash
OMP_NUM_THREADS=4 ./executable1 # Runs on 4 threads.
OMP_NUM_THREADS=1 ./executable2 # Runs on 1 thread, i.e. in serial.
```

* *HelloWorld* Two simple examples showing the use of the basics of `OpenMP`, such as `#pragma omp parallel`, `#pragma omp critical`, `omp_get_thread_num()` and so on.

* *DataRace* An example showing a possible issue of parallel programs, *i.e.* data race: each entry of an array is filled in parallel (using `#pragma omp for`) with a value based on the corresponding next entry. Depending on which thread first fills each entry, the result may be wrong. The serial and the parallel output are compared.

* *PI* An example of computing an integral using the composite midpoint rule in parallel, where the subintervals are assigned to different threads. The use of `#pragma omp for reduction` is also shown.
