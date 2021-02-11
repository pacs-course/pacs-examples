#Utilities for Block preconditioning of saddle point matrices#

This directory contains a few classes for block preconditioners for SaddlePoint matricies arising from the discretization of the Darcy problem

**Note** It makes use of the utilities contained in the `spectra` library, contained in the folder `LinearAlgebra/spectra`. You need to have it installed to be able to compile the code in this directory. To this aim, you go in `LinearAlgebra/spectra` and run `make install`. 

**Main Content:**

- `BasicType` The trait containing the main types used throughout the code
- `dumpMatrices` Some utilities to write on files the matricies that form the saddle point block-matrix
- `eigenSolver` Interface with spectra to compute eigenvalues of the matrix used in this example
- `preconditioner` The block preconditioners used in the cited paper.
- `preconditionerFactory` A simple factory to choose the preconditioner run-time
- `Range_support` You want to be able to use `begin()` and `end()` to traverse an eigen matrix. Here we go.
- `SaddlePointMat` contains the class for saddle point matrices of the form explained in the paper
- `SaddlePointUtilities`  Some utilities to read parameter and location of the matrices from a file. We use `GetPot` (available in the main `Utilities` folder (you have to install it using `make install`, as usual).
- `SparseBlock` Extracts a block from a sparse Eigen Matrix.


#Bibliography#

 [Antonietti,P.F.; De Ponti, J.; Formaggia, L.; Scotti, A. Preconditioning techniques for the numerical solution of flow in fractured porous media](https://doi.org/10.1007/s10915-020-01372-0)
 
##What do I learn from this code?##
Well, this is a complete working code, rather complex becouse it is the result of several people working on it. but from it you can get

- The use of getpot to store parameters and information read from a file
- A use of the spectra library for computing eigenvalues of dense and sparse Eigen matrices. An alternative to the native tools of the library (which are really basic)
- How to create a simple object factory. Less sophisticated than the one in `GenericFactory`, but in many cases yo ujust need something simple but working.
- Some non standard use of the Eigen library
- How to build preconditioners suitable for the iterative methods in `IML_Eigen`
 
 
 
 **Note:** The code is evolving, the description contained here may not correspond exactly to the actual content of the folder. 