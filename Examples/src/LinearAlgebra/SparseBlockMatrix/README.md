# A tool for block matrices #

With this tool you can cleare block matrices of any size, whose block is
an Eigen sparse matrix.

## How does it work ? ##
You can construct a block matrix M using the constituting blocs M_ij.

You have first to construct an empty block matrix, specifying the block sizes.  In particular,
you have to give the number of rows and column of each block.

Then, you can add (maybe by moving) the actual sparse mmatrix. You may indicate the a matrix block is actually the transpose of another block, without the need to create the transpose!

Eventually you have methods for

- Accessing a given block;
- Matrix vector multiplication M*v
- Vector matrix multiplication v*M (v row vector)
- Computation of Frobenius norm 
- Extracting the complete sparse matrix as a single matrix

The blocks are stored using *shared pointers to Eigen matrices*, this is the trick needed to avoid storing 
the transpose of a given block. The transposed block is a pointer to the block to transpose and I just keep track that that block refer to a transposition. 

I rely on the fact that Eigen matrices do (finally!) support move semantic,
so use `std::move` if you vant to move a block inside the block matrix and not just copy it.
 
Some checks are made to verify that the dimensions of a stored block corresponds to the block size allocated during construction.

## What do I learn here? #  
- Some tools of th Eigen library
- A use of `shared_ptr`
