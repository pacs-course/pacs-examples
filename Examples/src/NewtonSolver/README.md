# Newton And Quasi-Newton Solvers

This directory contains a small library and a test program for solving
nonlinear systems
\[
F(x)=0, \qquad F:\mathbb{R}^n \to \mathbb{R}^n,
\]
with Newton and quasi-Newton techniques built on top of `Eigen`.

The code is organized around two main abstractions:

- `Newton`: drives the nonlinear iteration, stopping tests, optional
  backtracking, and user callback.
- `JacobianBase`: polymorphic interface used to compute the Newton-like
  correction. Concrete implementations include exact, discrete, and
  quasi-Newton updates.

## Main Components

- `NewtonTraits.hpp`
  Defines the common types used by the whole module:
  `ArgumentType`, `ReturnType`, `NonLinearSystemType`,
  `JacobianMatrixType`, and `JacobianFunctionType`.

- `NewtonMethodsSupport.hpp`
  Defines `NewtonOptions` and `NewtonResult`, the aggregates used to pass
  solver parameters and collect the outcome of a run.

- `Jacobian.hpp` and `Jacobian.cpp`
  Implement the Jacobian hierarchy:
  `JacobianBase`, `FullJacobian`, `DiscreteJacobian`,
  `IdentityJacobian`, `BroydenB`, `BroydenG`, and
  `Eirola_Nevanlinna`.

- `JacobianFactory.hpp`
  Provides `JacobianKind` and `make_Jacobian()` to create a concrete
  Jacobian object at runtime.

- `Newton.hpp` and `Newton.cpp`
  Implement the solver itself. `NewtonVerbose` shows how to specialize the
  `callback()` hook to inspect the iterations.

- `main_Newton.cpp`
  Example driver that solves a test nonlinear system with several Jacobian
  strategies and prints convergence information.

- `NewtonSolver.tex`
  Additional mathematical and design notes. Running `pdflatex` on this file
  produces a more formal description of the algorithm.

## Building

This folder uses the project-wide build infrastructure from the repository
root `Makefile.inc`. In practice you should run commands from this directory.

Requirements:

- a C++23-capable compiler
- `Eigen`
- the common PACS build variables resolved by the top-level make setup

Typical commands:

```bash
make
make exec
make static
make dynamic
make alllibs
make clean
make distclean
```

What they do:

- `make exec`
  Builds the static library if needed and links the example executable
  `main_Newton`.

- `make static`
  Builds `libNewton.a` and the executable.

- `make dynamic`
  Builds `libNewton.so` and the executable.

- `make alllibs`
  Builds both `libNewton.a` and `libNewton.so`.

- `make clean`
  Removes object files and executables.

- `make distclean`
  Also removes generated libraries, dependency files, and documentation
  artefacts.

Build configuration:

- `DEBUG=yes` keeps debug information.
- `DEBUG=no` enables `-DNDEBUG` and optimized compilation.
- `LIBTYPE=STATIC` or `LIBTYPE=DYNAMIC` selects the library kind where
  relevant.

Examples:

```bash
make exec
make static DEBUG=no
make dynamic DEBUG=no
make alllibs DEBUG=no
```

## Installation

The target

```bash
make install
```

copies:

- all local headers (`*.hpp`) into `$(PACS_INC_DIR)`
- `libNewton.a` and `libNewton.so` into `$(PACS_LIB_DIR)`

Before installing, build the libraries first:

```bash
make alllibs
make install
```

If you compile in release mode and rely on shared libraries installed in the
project library directory, make sure the runtime loader can find them. In the
rest of this repository that is usually done by exporting `LD_LIBRARY_PATH`
appropriately.

## Running The Example

After building:

```bash
./main_Newton
```

The driver tests:

- exact Newton with a user-provided Jacobian
- Newton with a finite-difference Jacobian
- Broyden bad
- Broyden good
- Eirola-Nevanlinna

The verbose callback prints residual norm and step length at each iteration.

## Design Notes

- `Newton` stores the nonlinear system by value and the Jacobian strategy
  through a `std::unique_ptr<JacobianBase>`.
- Copy operations are deliberately disabled because the Jacobian hierarchy is
  polymorphic and does not implement cloning.
- The callback mechanism is the intended extension point if you want custom
  logging, statistics, or monitoring during iterations.

## Possible Issues To Keep In Mind

The current code builds and the sample run converges, but there are a few
points worth noticing when reusing or extending it:

- In `Newton::solve()`, when backtracking is active the stored step length is
  computed as `||delta||` before damping. If `lambda < 1`, the accepted step is
  actually `||lambda * delta||`, so the reported value and the convergence test
  may be stricter than intended.

- In `BroydenB::setB()`, `BroydenG::setB()`, and
  `Eirola_Nevanlinna::setB()`, calling `setB()` with the default empty matrix
  disables first-time initialization by setting `firstTime = false`. A later
  `solve()` can then operate with an invalid internal state unless a valid
  initial matrix is supplied explicitly.

- The quasi-Newton classes keep internal mutable history (`B`, `previousX`,
  `previousR`), so reusing the same Jacobian object across different problems
  requires care. Resetting the nonlinear system via `setNonLinSys()` helps, but
  a full restart policy should be part of the calling code.

## What You Learn Here

- use of traits to centralize algebraic types
- separation between nonlinear iteration and Jacobian strategy
- runtime selection of algorithms through a small factory
- use of aggregates for solver options and results
- use of callbacks as a lightweight extension mechanism

For a more detailed walkthrough of every file and of the intended purpose of
the module, see `ContentExplanation.md`.
