# Parallel matrix-vector product #

Matrix-vector product is one of the classic parallel algorithm. Typically, one splits
the matrix by assigning a given number of rows to each process, while the vector, being less memory demanding, is kept replicated.

Here we use a simple matrix linearised as a vector of double, where the element (i,j) of the matrix is mapped to v[j +n_cols*i], being n_cols the number of columns. Alternativley, one may use specialised libraries for matrices and vectors.

In this example we show also how one can have a hybrid implementation, where the each MPI process is itself further parallelised with OpenMP. 

This may optimise the use of resources in hybrid architecture, for instance a clusters of PCs where each processor is multicore. An alternative, not presented here, is to use at low level vailable higly optimised linear algebra software, for instance the [BLAS](https://netlib.org/blas/).

The code  compute the rimings (using MPI timing tools) of the different parts of the procedure separately. One may note how the scatter of matrix elements is rather costly. Not only, having to store the full matrix on the root process is memory demanding. In practical situation, one tries to build the matrix locally, whenever possible.

# What do I learn here? #

Some MPI procedures: Gathes, Scatter, Broadcast, Barrier... A trick to output local data in the order of the ranks.



