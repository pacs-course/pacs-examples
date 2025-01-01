# Packages for linear (and non linear) algebra #

* `CppNumericalSolvers` Solvers for optimization of a given cost functional. A modified copy of that in  `git@github.com:PatWie/CppNumericalSolvers.git`

* `IML_Eigen` Iterative solvers for linear systems of equations. My
  porting to Eigen of the template class of `IML++`.
  
* `MatrixData` A few matrices in Matrix Market format, useful to test the code contained  in `IML_Eigen`

* `MatrixMarketReadersMatlab` As the name says.

* `redsvd-h` The C++ porting (using Eigen as Matrix Library) of
  redsvd, a tool for reduced rank approximation of a Matrix using
  random matrices. A modified version of the one in
  `git@github.com:ntessore/redsvd-h.git`.
  
* `spectra` Efficient solvers for estimating the spectra of (possibly
  large) sparse matrices. A modification of that in
  `https://github.com/yixuan/spectra.git`.

* `SparseBlockMatrix` A set of utilities to create and handle general block matrices whose blocks are sparse Eigen matrices

* `Thomas` The Thomas algorithm for the solution of tridiagonal linear systems of equations. It's an algorithm with O(n) complexity

* `ZeroOfPolynomial` An implementation of the Newton-Horner algorithms for finding the zeroes of a polynomial.
It works also for polynomial on the complex field. 


*See also the README files in each folder. Note that in folders that are fork of third party software the README file explaining the example for the course is called README_PACS.md*
