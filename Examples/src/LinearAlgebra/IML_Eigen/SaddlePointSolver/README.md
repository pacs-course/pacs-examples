# Utilities for Block Preconditioning of Saddle-Point Matrices

This directory contains a self-contained example for solving sparse saddle-point
linear systems arising from mixed formulations of Darcy-type problems, with
special attention to fractured porous media. But, in fact, may be extended to any classig
saddle point system with matrix of the form
\[
\begin{bmatrix}
M & B^T \\
B & T
\end{bmatrix}
\]

The code reads the three block
matrices `M`, `B`, and `T` from Matrix Market files, wraps them in a block
operator, and then solves the resulting linear system with either:

- a direct sparse solver (`umfpack`);
- one of the iterative solvers available in `IML_Eigen`;
- an eigenvalue analysis mode used to inspect spectral properties.

The implementation is centered on block preconditioners. Instead of assembling
one large dense preconditioner, each class stores the sparse factors or
diagonal approximations needed to apply `P^{-1}` efficiently inside Krylov
iterations.

**Note** The code uses the `Spectra` utilities available in
`LinearAlgebra/spectra`. To build the eigenvalue-analysis mode you need that
library installed first. From `LinearAlgebra/spectra`, run `make install`.

## Purpose and Usage

The folder provides a small research-style driver that lets you test different
preconditioners and solvers on the same saddle-point problem. Its main purpose
is to compare block preconditioning strategies on systems of the form

\[
\begin{bmatrix}
M & B^T \\
B & T
\end{bmatrix}
\]

where `M` is typically the velocity/mass block, `B` is the coupling block, and
`T` contains lower-dimensional or stabilization contributions. For some
preconditioners the code can also convert this 2x2 structure into a 3x3
"double saddle-point" form that separates cell and fracture unknowns.

Typical workflow:

1. Prepare a GetPot input file such as `data.pot`.
2. Set the paths of the Matrix Market files `M.mat`, `B.mat`, and `T.mat`.
3. Choose a solver:
   `umfpack`, `gmres`, `gmresr`, `fgmres`, `minres`, `tminres`, or
   `eigenvalues`.
4. Choose a preconditioner:
   `Identity`, `Diagonal`, `BlockDiagonal`, `BlockTriangular`, `ILU`, `HSS`,
   `DoubleSaddlePoint`, or `DoubleSaddlePointSym`.
5. Build and run `main`, optionally passing a custom data file with `-i`.

The executable reads the input data, loads the sparse blocks, constructs the
`SaddlePointMat` object, instantiates the selected preconditioner through the
factory, and finally runs the requested algorithm. In solve mode it builds a
manufactured right-hand side `b = A x_exact` and reports iteration count,
achieved tolerance, relative solution error, and relative residual. In
`eigenvalues` mode it computes extremal eigenvalues of the relevant blocks and
of the assembled full matrix.

Example:

```bash
make
./main
./main -i data_extra.pot
```

The default configuration file is `data.pot`. The sample file in this folder
points to the matrices stored under `matrices/network1/`.

## Main Content

- `BasicType.hpp`: common scalar, matrix, vector, and enum aliases used across
  the module.
- `dumpMatrices.[hc]pp`: utilities to export the assembled blocks for external
  inspection.
- `EigenSolver.hpp`: interface to `Spectra` for sparse eigenvalue computations.
- `preconditioner.[hc]pp`: implementation of the block preconditioners used by
  the example.
- `preconditionerFactory.hpp`: runtime factory that maps the input choice to a
  concrete preconditioner class.
- `RangeSupport.hpp`: helper utilities to iterate over Eigen objects with
  generic algorithms.
- `SaddlePointMat.hpp`: sparse block representation of the saddle-point
  operator.
- `SaddlePointUtilities.hpp`: parsing and validation of the GetPot input file,
  plus helper functions for configuring the selected preconditioner.
- `SparseBlock.hpp`: extraction of a sparse sub-block from an Eigen sparse
  matrix.
- `main.cpp`: the executable driver.

## Bibliography

[Antonietti, P. F.; De Ponti, J.; Formaggia, L.; Scotti, A. Preconditioning techniques for the numerical solution of flow in fractured porous media](https://doi.org/10.1007/s10915-020-01372-0)

## What Can Be Learned from This Code?

This example is useful if you want to study:

- how to read and validate a parameter file with `GetPot`;
- how to store a saddle-point operator in block form rather than as one
  monolithic sparse matrix;
- how to assemble Schur-complement-based block preconditioners;
- how to dispatch solvers and preconditioners at runtime through enums and a
  small factory;
- how to interface Eigen-based sparse algebra with `Spectra` for spectral
  analysis.
