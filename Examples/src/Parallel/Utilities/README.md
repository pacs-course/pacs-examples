# General Utilities for parallel computing #

## Partitioner ##

In a data decomposition strategy for parallel computing you normally have to partition a vector *v* of *n* elements into *p* chunks to be distributed to *p* taskes (thread or processes). If *n* is a multiple of *p* the partitioning is trivial: you give a chunk of *n/p* elements to each task.

If *n* is not a multiple of *p*, a balanced decomposition provides *n/p +1* elements to *n mod p* tasks  and *n/p* elements to the remaining *n -n mod p*, where *n/p* indicates the *integer division* between *n* and *p*.

Given *n* and *p* the type of information we may wish to get is

- The starting position of the element of i-th chunk of vector *v*;
- The end position of the i-th chunk, or, using the usual convention, the last plus one, so that the elements of the i-th chunk are in the semi-open interval *[first(i),last(i)[*;
- The chunk index of the j-th element of the vector;
- The `counts` and `displacements` vectors to be used in `MPI_Gatherv` and `MPI_Scatterv` for distributing/collecting the vector elements.

The partition can be made in many ways. In particular

- The *grouped* strategy sets the first *n mod p* chunks with dimension *n/p +1*, the other of dimension *n/p*. An alternative is to have the last of  dimension *n/p +1*. In any case, the vector is split into two consicutive portions (one of which can be empty) that provide chunks of equal size. It is very simple to implement, however the operation of extracting the chunk index of an arbitrary vector element is complicated. 
If this information is needed it is better to resort to the other strategy.
- The *distribution* stategy: the number of elements in the *i-th* chunk is given by

	[(i+1) * n] / p - [i * n] / p
and it may be shown that it is equal to either *n/p* or *n/p+1* and the sum for *i=0,...,p-1* is exactly *n*.
The only advantage is that the operation of extracting the chunk index of an arbitrary vector element
is now simpler, so this partitioning is to be preferred when this operation is needed frequently.

In `partition.hpp` you have two classes that implements the two partitioning techniques.

In the same `partition.hpp` file, we have a template function, `counts_and_displacements()`, that takes a valid partitioner and returns the corresponding `counts` and `displacements` vectors, ready to be used in a `MPI_Gatherv` or `MPI_Scatterv`.

In the same file you have the class template `MatrixPArtitioner` that can be used for the partition of a full matrix. It relies on the `Partitioner` class and provides the same information as the `Partitioner` class, but for a matrix.

## mpi_utils ##
A set of utilities taken from the Muster code developed at Lawrence Livermore Labs. For details, see http://github.com/LLNL/muster. Take note of the licence statement in the code.

In particular it defines `MPI_SIZE_T`, useful when you want to send object of std::size_t type. 

## The utility `SafeMPI.hpp` ##

The new `override` clause is very useful, but since it has been introduced in the language only recently it is not used in several legacy code, in particular some classes loaded by `mpi.h`.
In the course, we use by default the very useful warning `-Wsuggest-override` that indeed tells where, according to the compiler, you should add `override`.

When you compile with the usual setting a file that includes `mpi.h` you get a lot of annoying warnings, This file uses `#pragma GCC diagostic` to wrap the inclusion of `mpi.h` and suppress the warning related to `override` and another one related to a potentially dangerous cast of function pointers (but we trust mpi programmers, don't we?).



# Instructions #

If you just type `make all`, or simply `make`, you compile a test code for the partitioner. **If you do `make install` the content of this folder goes in the correct `include` folder and will be available for other examples.**
