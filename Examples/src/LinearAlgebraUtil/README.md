# A Set of utilities for linear algebra #

To install the utilities in the PACS default folders do

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

* `thomas.hpp` Thomas algorithm to solve tridiagonal systems. It contains also a version for
periodic systems, and a tool to multiply a tridiagonal matrix givne by 3 vectors and a vector.

* `RotatingMatrix` Like rotating vector, but now the vectors are the compuns of an Eigen Matrix. In practice you have an Eigen matrix with a maximal number of columns
you may add a column to the end of the matrix and if the maximal number of columns is reached, the columns are shifted to the left to give room to the new column. As a consequence the first column is canceled.

* `mathUtils.hpp` It contains some utilities for linear algebra operation. For instance dot product and norm that wotk on a very generic setting. *It also extends arithmetic operations to std::vector!*
It implement parallelism either with C++ native paralell algorithm or by using openMP. Have a look to the *Makefile* to change between the two options when compiling the test. I general if you compile with `-fopenmp` you get the openmp version. Otherwise you have the native parallelism (remeber to link with libtbb!)

* `basicOptimization` Golden search for unconstrained minimization of a scalar function. And a simple implementation of a function to bracket a minimum. 

* `basicZeroFun.hpp` Basic routines for the zero of a scalar function. Bisection, secant, Newton, Brent, also a tool for bracketing a zero with odd multiplicity. Only scalar.

* `vectorConcept` a concept tp test if a tempalte parameter is a std::vector<T> with T algebraic.
 
In `test/` you have tests for all the utilities here contained, do

    cd test
    make DEBUG=no

and try to run them. Look at the code for examples on how to use these
utilities.
 
