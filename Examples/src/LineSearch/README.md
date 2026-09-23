# Line-search optimization methods

This folder contains an educational C++ implementation of iterative methods for
minimizing a scalar function

\[
  \min_{x \in \mathbb{R}^n} f(x),
\]

optionally subject to component-wise bounds \(l \leq x \leq u\).  Each outer
iteration computes a search direction, chooses a step length with backtracking,
and updates the current point.  The implementation uses Eigen dynamic vectors
and matrices and supports user-supplied, finite-difference, or automatic-
differentiation derivatives.

The principal executable is `main_linesearch`; it solves a small two-variable
example and shows how to select the descent-direction algorithm, enable bounds,
and supply derivatives. `test_gradientAD` is a separate check of the automatic
differentiation wrapper.

## Build and run

From this directory:

```sh
make
./main_linesearch
./test_gradientAD
```

`main_linesearch` reads the optional JSON files in the current directory:

- `linesearch_options.json` configures the Armijo backtracking parameters and
  the direction name;
- `optimization_options.json` configures stopping tolerances and the maximum
  number of outer iterations.

The Makefile enables verbose iteration diagnostics. Build artefacts can be
removed with `make clean`.

## What is included

The following direction names can be selected through
`LineSearchOptions::descentDirection`:

| Name | Method | Derivatives required |
| --- | --- | --- |
| `GradientDirection` | Steepest descent | Gradient |
| `NewtonDirection` | Newton step | Gradient and Hessian |
| `BFGSDirection` | BFGS Hessian approximation | Gradient |
| `BFGSIDirection` | BFGS inverse-Hessian approximation | Gradient |
| `BBDirection` | Barzilai--Borwein scaled gradient | Gradient |
| `CGDirection` | Nonlinear conjugate gradient (Polak--Ribiere) | Gradient |

The line search enforces the Armijo sufficient-decrease condition. For a
complete explanation of the algorithms, the class design, derivative options,
and the handling of box constraints, see [Description.md](Description.md).
The accompanying theory note is [LineSearchTheory.tex](LineSearchTheory.tex).

## Main files

- `LineSearchSolver.hpp/.cpp` — outer optimization loop, Armijo backtracking,
  and projection onto box bounds.
- `DescentDirectionBase.hpp`, `DescentDirections.hpp/.cpp` — common interface
  and the concrete direction strategies.
- `DescentDirectionFactory.hpp/.cpp` — registration and construction of a
  strategy from its string name.
- `LineSearch_traits.hpp` — common Eigen and callable types.
- `LineSearch_options.hpp`, `Optimization_options.hpp` — configuration and
  problem-data aggregates, including JSON readers.
- `GradientFiniteDifference.hpp` — forward, backward, and centered numerical
  gradients.
- `GradientsAD.hpp` — reverse-mode autodiff wrapper for cost, gradient, and
  Hessian callables.
- `main_linesearch.cpp` — worked example.

## Scope

This is a compact teaching library rather than a production optimizer. It
supports simple box bounds only; it does not implement general constraints,
Wolfe curvature checks, trust regions, or limited-memory quasi-Newton methods.
The boxed-constraint behavior and its current caveats are documented in
[Description.md](Description.md).

# What do I learn here?

- An Example of several line serch strategies for function optimization
- The use of object factories, json interface
