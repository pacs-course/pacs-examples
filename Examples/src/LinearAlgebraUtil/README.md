# A Set of utilities for linear algebra #

To install the utilities in the PACS default folders do

    make install

Content

 	
* `RotatingVector` A class that implements an array that stores only
	the last `N` entries added (perhaps the name is not very
	appropriate, we are not rotating anything). The array has `N`
	element and the insertion of the `N+1` element will cause the first
	element to be discarded, all the elements shifted by one to the
	left and the new element added at the back of the `RotatingVector`.
    It is generic, so the RotatingVector may store any type.

* `thomas.hpp` Thomas algorithm to solve tridiagonal systems. It contains also a version for
periodic systems, and a tool to multiply a tridiagonal matrix given by 3 vectors and a vector.

* `RotatingMatrix` Like rotating vector, but now the vectors are the components of an Eigen Matrix. In practice you have an Eigen matrix with a maximal number of columns
you may add a column to the end of the matrix and if the maximal number of columns is reached, the columns are shifted to the left to give room to the new column. As a consequence the first column is overwritten.

* `mathUtils.hpp` It contains some utilities for linear algebra operation. For instance dot product and norm that work on a very generic setting. *It also extends arithmetic operations to std::vector!*
It implements parallelism either with C++ native parallel algorithm or by using openMP. Have a look to the *Makefile* to change between the two options when compiling the test. In general if you compile with `-fopenmp` you get the openmp version. Otherwise you have the native parallelism (remember to link with libtbb!)

* `basicOptimization` Golden search for unconstrained minimization of a scalar function. And a simple implementation of a function to bracket a minimum. 

* `basicZeroFun.hpp` Basic routines for the zero of a scalar function. Bisection, secant, Newton, Brent, also a tool for bracketing a zero with odd multiplicity. Only scalar.

* `vectorConcept` a concept to test if a template parameter is a std::vector<T> with T algebraic.
 
In `test/` you have tests for all the utilities here contained, do

    cd test
    make DEBUG=no

and try to run them. Look at the code for examples on how to use these
utilities.
 
