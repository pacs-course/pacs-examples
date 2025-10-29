# RKFSolver — Methodology and Code Structure

This document explains the methodology implemented in the `RKFSolver` sources and describes the structure and main responsibilities of the key files and symbols contained in the folder. The explanation is based on the `RKF.hpp` implementation and the `main_rk45.cpp` example included in the same folder.

## Purpose and high-level methodology

The code implements an adaptive Runge–Kutta–Fehlberg (RKF) integrator for ordinary differential equations (ODEs). The RKF object is a class template that is parameterized by a Butcher tableau type `B` (a compile-time description of the RK scheme) and by a `KIND` parameter which selects whether the integrator works with scalar, vector or matrix variables (the library currently uses `RKFKind::SCALAR` and `RKFKind::VECTOR`).

Key ideas implemented:
- Adaptive step-size control using embedded RK pairs (two formulas of different orders) to estimate local truncation error and adapt `h`.
- Error control tuned to either absolute/relative behavior and total integration interval.
- Support for explicit explicit RK schemes and diagonally implicit RK (DIRK) schemes by invoking a Newton solver when a stage requires solving a nonlinear equation.
- The Butcher tableau `B` is supplied as a compile-time constant (a type) so that stage loops and coefficients are known at compile time.

## Main types and interfaces

- `template <apsc::ButcherArrayConcept B, RKFKind KIND = RKFKind::SCALAR> class RKF`:
  - `VariableType` and `Function` are aliases derived from `RKFTraits<KIND>`; `VariableType` is `double` for scalars or `Eigen::VectorXd` for vectors.
  - The integrator is constructed with a forcing function `F` (callable) matching the `Function` signature.
  - `RKF::operator()(T0, T, y0, hInit, tol, maxSteps)` runs the integration and returns an `RKFResult`.

- `struct RKFResult`:
  - Holds `time` (vector<double>), `y` (vector<VariableType>) with state values at recorded times, `estimatedError`, `failed`, and counters for `expansions` and `contractions`.
  - A `friend` `operator<<` is defined inside `RKFResult` to print results in a gnuplot-friendly format; the implementation branches on `KIND` to format scalar vs vector outputs.

- `apsc::Newton newtonSolver` and `apsc::NewtonOptions newtonOptions`:
  - `newtonSolver` is used when a stage is implicit (determined by `ButcherTable.implicit()`). The solver is configured from `newtonOptions`.

## Core implementation details

- RKFstep (private):
  - Implements a single step of the RK scheme. It computes stage values `K[i]` using the Butcher arrays `A`, `c`, and two sets of weights `b1` and `b2` (the embedded pair).
  - For explicit schemes `K[i] = f(t + c[i]*h, value) * h`.
  - For implicit (DIRK) schemes, when `ButcherTable.implicit()` is `true` and `KIND` is `VECTOR`, a Newton nonlinear solve is performed for the stage (the code sets the `newtonSolver` non-linear system and calls `solve`).
  - Returns the pair of approximate solutions computed with the two rules (low/high accuracy) so the error can be estimated.

- operator() (integration loop):
  - Maintains `t`, `h`, `ycurr` and performs step attempts until `t >= T` or step count exceeds `maxSteps`.
  - Computes `errorPerTimeStep` (a scaled tolerance depending on the scheme order and integration interval).
  - Uses ratio `errorPerTimeStep/currentError` and factors based on the scheme order to decide whether to accept, expand or reject the step:
    - If `ratio >= 1.0` the step is accepted; `h` may be expanded (bounded by `expansionFactor`).
    - If `ratio < 1.0` the step is rejected and `h` is reduced (bounded by `hmin`).
  - Tracks `estimatedError`, and counts `expansions` / `contractions`.
  - `hmin` is chosen as a safe lower bound relative to machine epsilon and the time interval.

## Printing and examples

- The `friend operator<<` inside `RKFResult` prints header information (number of steps, contractions/expansions, min/max `h`) and then the `t y` lines. For vector states each component of the vector is printed on the same line after the time.

- `main_rk45.cpp` demonstrates three uses:
  1. A scalar ODE `y' = -10 y` integrated with `RKFScheme::RK45_t` showing error check (compares to closed-form solution) and writing `result2.dat`.
  2. A 2D Van der Pol oscillator (vector) integrated with an implicit DIRK variant `ESDIRK12_t` and writing `resultVDP.dat`.
  3. A very-stiff vector example integrated with `ESDIRK34_t` (also vector) writing `resultstiff.dat`.

Each example constructs `apsc::RKF<...>` with a lambda forcing function, runs the solver via `solver(t0, T, y0, h_init, errorDesired, maxSteps)`, prints summary to `stdout` and exports time-series results to files via the `operator<<`.

## Build and run

To build the `RKFSolver` example (from the project root):

```bash
# build only the RKFSolver folder (observed makefile path)
make -C src/RKFSolver
# the produced executable in the example is `main_rk45` (linked against -lNewton)
./src/RKFSolver/main_rk45
```

Requirements:
- C++20 (code uses concepts and designated initializers)
- Eigen (the project includes `-I/usr/local/include/eigen3` in the example build line)
- The `Newton` library from the project (linked as `-lNewton` in the RKFSolver Makefile)

## Design notes, caveats and suggestions

- The integrator assumes that the Butcher table `B` provides compile-time constants `A`, `c`, `b1`, `b2` and `order()` and a boolean `implicit()` accessor.
- Implicit stages are supported only for `KIND == VECTOR` in the current implementation; invoking implicit RK for scalar or matrix kinds intentionally throws.
- The `friend operator<<` inside the nested `RKFResult` type was added to avoid template-deduction problems when streaming `RKFResult` objects from callers.
- If you plan to expose `RKF` in a public API, consider:
  - Providing a stable `Result` type alias at namespace scope for easier printing and function signatures.
  - Adding unit tests for correctness on manufactured solutions and for the adaptive-step acceptance/rejection policy.
  - Making tolerance handling (absolute vs relative) configurable.

## File map (responsibilities)
- `RKF.hpp` — main template implementation of the RKF integrator and `RKFResult` printing helpers.
- `main_rk45.cpp` — example driver showing scalar and vector usage and how to write results to files.
- `ButcherRKF.hpp` — Butcher table definitions and concept used to parameterize `RKF` (compile-time scheme descriptions).
- `RKFTraits.hpp` — maps `RKFKind` to `VariableType` and `ForcingTermType`.
- `Newton.hpp`, `JacobianFactory.hpp` — utilities used to solve nonlinear stage equations for implicit schemes.
- `Makefile` — build instructions for compiling the RKFSolver example and linking against the Newton library.
-  README.md — A file containing further information about the RKFSolver module and its usage.