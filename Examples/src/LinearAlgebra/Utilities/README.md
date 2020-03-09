#A Set of utilities for linear algebra#

To install the utilities in the PACS default folders do

    make alllibs DEBUG=no
    make install

Content

* `CooExtractor.hpp` It allows to create a sparse matrix which is
 	basically of the form of a vector of indexex `(i,j)` and the
 	corresponding values.  We have utilities to write the matrix in
 	this format in Octave/Matlab format or convert it to a sparse
 	eigen matrix Additional utilities allows to write an eigen matrix
 	in a format readable by Octave/Matlab format. *Note* In
 	`../MatrixMarketReadersMatlab` you have Matlab script to read
 	matrices in Matrix Market format directly into Matlab/Octave.
    

* `MM_readers` Utilities fo read matrices in Matrix Market format
 	into Eigen. Matrix Market format is a format widely used to store
 	sparse matrices, Search Matrix Market on the web for
 	documentation. A Note: Eigen has its own readers for matrix
 	market. I wrote this utility because native Eigen reading
 	utilities did not recognize MatrixMarket matrices in symmetric
 	format. *Note* Since version 3.4 things are evolving and the
 	interface for matrix market marix of Eigen is getting
 	better. Maybe a future release of Eigen will make the functions
 	contained in this file obsolete and one should use the ones
 	provided by Eigen directly.
 	
* `RotatingVector` A class that implements an array that stores only
	the last `N` entries added (perhaps the name is not very
	appropriate, we are not rotating anything). The array has `N`
	element and the insertion of the `N+1` element will cause the first
	element to be discarded, all the elements shifted by one to the
	left and the new element added at the back of the `RotatingVector`.
    It is generic, so the RotatingVector may store any type.
	
* `Utility` An utility to extract the graph of a matrix and another
  utility to apply symmetric permutation on a Eigen sparse matrix. We
  have also an utility to apply RCMK reordering of a matrix.
 
In `test/` you have tests for all the utilities here contained, do

    cd test
    make DEBUG=no

and try to run them. Look at the code for examples on how to use these
utilities.
 
