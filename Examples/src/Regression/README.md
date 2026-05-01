# Regression Example

This directory contains a small regression framework built around:

- polynomial basis functions
- a polynomial regression evaluator
- an MSE cost function
- a QR-based linear regression solver
- an optional adapter for `CppNumericalSolvers`

The full explanation is in `README.tex`.

## Build the notes

Run:

```sh
make README.pdf
```

## Build the example

If `CppNumericalSolvers` is installed in the PACS include tree, run:

```sh
make
```

This builds both the direct QR-based solver and the optimization-based path in
`main.cpp`.

If the optional solver is not installed, build without that part:

```sh
make CPPFLAGS_EXTRA=-DNOCPPSOLVER
```

## Optional dependency

The file `CostFunctionProxyCppNumSolver.hpp` requires the
`CppNumericalSolvers` headers. Install them with:

```sh
git submodule update --init --recursive
cd ../LinearAlgebra/CppNumericalSolvers
make
```

Then rebuild this directory normally with `make`.
