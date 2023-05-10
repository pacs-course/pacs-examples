# An example of parallel matrix #

*(if you have a compilation error see the note at the bottom)*

In this example we illustrate a class template that represents a parallel (full) matrix and is able to perform matrix-vector multiplication in parallel in an MPI environment. It supports two types of matrix partitioning: `block row` and `block column`. I recall that they consist in distibuting the rows of the matrix, respectively the columns,  to the different processes so that the local number of rows (columns) is (nearly) the same.

The choice of making row-based or column-based partitioning is automatically determined by the type of the `Matrix` template parameter. If it is a row-wise ordered matrix the row block partition is adopted, and viceversa.  The class has a default constructor and you can pass the global matrix to be partitioned and the MPI commuinicator using the method `setup`. Indeed, we have adopted the strategy where the master process (by default of rank 0) has the global matrix (a different technique is to have each process set the local matrix, maybe by reading from a file, but here it is not implemented).

Let's see an example, taken from the test program present in this directory.

```
using RowMatrix=apsc::LinearAlgebra::Matrix<double,ORDERING::ROWMAJOR>;
RowMatrix mr; // the global matrix 
  if(mpi_rank==0)
    {
    ... //fill the global matrix with values
    }
  apsc::PMatrix<RowMatrix> pmr; // the parallel matrix class
  pmr.setup(mr,MPI_COMM_WORLD);
```
We have used for convenience a type alias to specify the `Matrix<>` type instance corresponding to a row-major matrix of doubles.

Now all processes have their local version and, if you wish, you can erase the global matrix.
All processes must have a copy of the global vector with which we want to multiply the matrix, let's call it `v`. The parallel product is made with the method `product()`.

```
   pmr.product(v); 
```

The result may be extracted with the method `collectGlobal()`

```
 std::vector<douuble> result;
 pmr.collectGlobal(result);
```
In this form, **only the master process obtains the result**, in the other processes `result` is an empty vector. If you want all processes to get the result (more costly since you need an all-to-all communication), you do

```
 std::vector<douuble> result;
 pmr.AllCollectGlobal(result);
```
That's it.

## Note on the procedure for parallel matrix-vector product ##
Let the (global) matrix dimensions be `m X n` , `p` the number of processes, and `v` the vector to be multiplied with, of dimension `n`. 
We need to distinguish the two cases

** Row block partition case**
 
 - After the partition, each process `i` owns a local matrix `A_i` of dimension `l x n`, where `l` is at most `m/p +1`;
 - The local matrix-vector multiplication is carried out normally: `r_i=A_i v`, and `r_i` has dimension `l`;
 - The global solution vector is obtained by gathering the contribution `r_i` of each process using `MPI_gatherv` or `MPI_Allgaterv`. 
 
 ** Column block partition case**
 
 - After the partition, each process `i` owns a local matrix `A_i` of dimension `m x l`, where `l` is at most `n/p +1`;
 - The local matrix-vector multiplication is carried out by extracting from `v` the `l` rows corresponding to the global index of the columns of 
 `A` contained in `A_i`.  The local solution vector `r_i=A_i v_i` has dimension `m`;
 - The global solution vector is obtained by summing the local solution `r_i` of each process using `MPI_Reduce` or `MPI_Allreduce`. 
 
## Note on the result of the test program ##
First of all in the compilation I have not activated openMP. If you do, by adding `-fopenmp` to the `CXXFLAGS` in `Makefile.inc` you have an hybrid parallelization, since the class `Matrix` uses multithreading in the local matrix-vector product. However, not always hybrid parallelism works well in a computer not meant for hybrid parallel computing like a normal PC, that's why I have taken it away by default. Remember to compile with 

```
make DEBUG=no
```
if you want to get full optimization (do a `make clean` before, to be sure you recompile everithing).

Try to run the code using different number of processes (you are clearly limited by your machine...)

```
mpirun -n <NP> ./main_PMatrix
```

You may note that the setup time, i.e. the time taken to partition the matrix and distribute it, is dominant (at least in a normal PC computer), while the time for the actual matrix-vector parallel execution is rather satisfactory, showing a reasonable speedup even on a simple multicore PC. The conclusions are

- The technique where a master process distributes the global matrix partitiones to the processes implies a certain overhead due to the need of communicating large amount of data during the setup phase. In prectical situations, it's better to have each processor build its own local matrix, whenever possible.
- the parallel matrix-vector computation becomes more advantageous if repeated many times with the same matrix. Indeed, in this case the setup phase overhed may become irrelevant. Luckily, this is the most common situation, for instance in iterative techniques for the solution of linear systems.

## Note on the use of the class ## 

The `PMatrix.hpp` file includes other header files, in particular `partitioner.hpp`, `mpi_utils.h` and `Matrix.hpp`. To have them in the include directory of the Examples and thus available for the compilation of the test program using `make`, you need to

- go in the `Parallel/Utilities/` forlder and type `make install`
- go in the `Matrix/` folder and type `make install`
