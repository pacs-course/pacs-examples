# NewtonSolver Content Explanation

## Purpose Of The Folder

This folder provides a reusable C++ implementation of Newton and
quasi-Newton methods for nonlinear systems. The target problem is

\[
F(x)=0, \qquad F:\mathbb{R}^n\to\mathbb{R}^n.
\]

The code is meant both as a teaching example and as a small reusable library.
Its main educational value is that it separates:

- the nonlinear solver logic
- the strategy used to represent or approximate the Jacobian
- the linear algebra backend
- the solver configuration and returned diagnostics

The implementation uses `Eigen` for vectors, matrices, norms, and linear
solves.

## Overall Architecture

The core idea is to express a Newton-like iteration in the form

\[
x^{(k+1)} = x^{(k)} - \lambda_k d^{(k)},
\]

where the correction `d^(k)` is obtained by solving

\[
B(x^{(k)}) d^{(k)} = F(x^{(k)}).
\]

Here `B(x)` may be:

- the exact Jacobian
- a finite-difference approximation
- an approximation of the inverse Jacobian updated by a quasi-Newton formula
- a simple scaled identity, mainly for experimentation

This is why the code does not force a single Jacobian representation. Instead,
it uses a polymorphic interface named `JacobianBase`.

## File-By-File Explanation

### `NewtonTraits.hpp`

This file centralizes the algebraic types used by the module.

It defines:

- `ArgumentType`
  Dynamic column vector of doubles (`Eigen::Matrix<double, Dynamic, 1>`).

- `ReturnType`
  The return type of the nonlinear system. In this implementation it is the
  same as `ArgumentType`, because the solver expects `F: R^n -> R^n`.

- `NonLinearSystemType`
  A `std::function<ReturnType(const ArgumentType &)>` used to store the
  nonlinear map.

- `JacobianMatrixType`
  Dense dynamic matrix (`Eigen::MatrixXd`).

- `JacobianFunctionType`
  A callable object returning the Jacobian matrix at a given point.

Why this file matters:

- it keeps the rest of the code consistent
- it makes future changes to vector/matrix types localized
- it documents that the module is currently designed around dense linear
  algebra

### `NewtonMethodsSupport.hpp`

This file contains two simple aggregates.

`NewtonOptions` stores all parameters that affect the algorithm:

- `tolerance`
  Threshold on the step length.

- `minRes`
  Threshold on the residual norm.

- `maxIter`
  Maximum number of main nonlinear iterations.

- `backtrackOn`
  Enables or disables damping/backtracking.

- `stopOnStagnation`
  If true, the solver stops after repeated lack of residual decrease.

- `alpha`
  Sufficient decrease coefficient used in the Armijo-like backtracking test.

- `backstepReduction`
  Multiplicative reduction for the damping factor.

- `maxBackSteps`
  Maximum number of backtracking reductions per outer iteration.

- `lambdaInit`
  Initial damping parameter before backtracking starts.

`NewtonResult` stores the solver outcome:

- the final iterate
- the final residual norm
- the final step length
- the iteration count
- a convergence flag
- a stagnation flag

This design is useful because it avoids long argument lists and makes the
solver interface easier to extend.

### `Jacobian.hpp`

This is the main abstraction file for the Jacobian strategies.

#### `JacobianBase`

`JacobianBase` is the common polymorphic interface. It exposes:

- `solve(x, b)`
  Returns the correction associated with the local Jacobian model.

- `setNonLinSys(...)`
  Connects the Jacobian strategy to the nonlinear system when needed.

The important point is that the solver does not ask a Jacobian object to
explicitly return a matrix. It asks it to solve the local linear step. That is
more general and lets quasi-Newton methods hide their internal update logic.

#### `DiscreteJacobian`

This class approximates the Jacobian with centered finite differences:

\[
J_{:,i}(x) \approx \frac{F(x+h e_i)-F(x-h e_i)}{2h}.
\]

It optionally adds a regularization term `lambda * I`.

Its role:

- convenient fallback when the exact Jacobian is unavailable
- useful for prototyping
- numerically more expensive than an exact Jacobian because each column costs
  two evaluations of `F`

#### `IdentityJacobian`

This is a deliberately simple approximation. It returns a scaled residual and
therefore behaves like a crude fixed-point or Richardson-like correction.

Its role is mainly didactic:

- it shows that the solver only needs a correction strategy
- it is not meant to be a strong default for hard nonlinear problems

#### `FullJacobian`

This class stores an explicit Jacobian function supplied by the user.

At each solve step it:

1. evaluates the Jacobian matrix at the current point
2. solves the corresponding dense linear system with `fullPivLu()`

This is the most direct implementation of classical Newton's method.

#### `BroydenB`

This class implements the so-called "bad" Broyden update for an approximation
of the inverse Jacobian.

Internally it stores:

- `B`: approximation of the inverse Jacobian
- `previousX`: previous iterate
- `previousR`: previous residual
- `firstTime`: flag used to initialize the internal state

Even though the nickname is "bad", this method can still be effective and is
often more robust than the label suggests.

#### `BroydenG`

This is the "good" Broyden method. It also stores an approximation of the
inverse Jacobian and updates it using a different rank-one formula.

In practice it often converges faster than `BroydenB` on the provided example.

#### `Eirola_Nevanlinna`

This class implements another quasi-Newton update strategy based on a related
inverse-Jacobian correction formula.

Its presence is useful because it shows how new Jacobian strategies can be
added without changing the main `Newton` class.

### `Jacobian.cpp`

This file provides the actual implementation of the Jacobian strategies.

Important implementation points:

- `DiscreteJacobian::solve()`
  Builds a dense matrix column by column and solves it with `fullPivLu()`.

- `FullJacobian::solve()`
  Simply evaluates the supplied Jacobian function and solves the dense system.

- `BroydenB::solve()`, `BroydenG::solve()`, `Eirola_Nevanlinna::solve()`
  Update an internal inverse-Jacobian approximation and return the correction
  as `B * residual`.

The quasi-Newton classes use `mutable` members because their `solve()` method
logically belongs to a const solver interface, but their internal approximation
must evolve from one iteration to the next.

### `JacobianFactory.hpp`

This file provides a small runtime factory.

It defines:

- `JacobianKind`
  Enumeration of the supported Jacobian strategies.

- `make_Jacobian(kind, args...)`
  Helper that creates the requested concrete Jacobian and returns it as
  `std::unique_ptr<JacobianBase>`.

Why this is useful:

- calling code can choose the method at runtime
- the `Newton` constructor can remain compact
- the example in `main_Newton.cpp` can switch strategies without changing its
  high-level structure

### `Newton.hpp`

This is the public interface of the nonlinear solver.

Key design decisions:

- the nonlinear system is stored by value as a `std::function`
- the Jacobian strategy is stored polymorphically via
  `std::unique_ptr<JacobianBase>`
- copy construction and copy assignment are disabled
- move support is enabled

The class provides several constructors:

- with only a Jacobian object
- with nonlinear system plus Jacobian pointer
- with nonlinear system plus concrete Jacobian object
- with nonlinear system plus `JacobianKind`

This makes the class easy to use in different scenarios:

- explicit user control over the Jacobian object
- factory-based runtime selection
- concise construction in examples

The protected `callback()` method is an extension hook. Derived classes can
override it to inspect each iteration without modifying the main solver logic.

`NewtonVerbose` is a simple example of that pattern.

### `Newton.cpp`

This file contains the actual nonlinear iteration.

The workflow of `Newton::solve()` is:

1. unpack options and current state
2. reconnect the Jacobian to the current nonlinear system
3. evaluate the initial residual
4. check whether the initial guess is already acceptable
5. iterate:
   - compute the correction from the current Jacobian strategy
   - update the current iterate
   - optionally apply backtracking
   - test stagnation
   - test convergence
   - call `callback()`
6. return a `NewtonResult`

Convergence requires both:

- residual norm below `minRes`
- step length below `tolerance`

This is intentionally conservative and prevents declaring convergence only
because the residual is small while the correction is still large, or vice
versa.

### `main_Newton.cpp`

This is the example driver.

It defines:

- a test nonlinear system in three variables
- its exact Jacobian
- an initial guess
- a `NewtonVerbose` solver

Then it runs the same problem with several Jacobian strategies:

1. `FullJacobian`
2. `DiscreteJacobian`
3. `BroydenB`
4. `BroydenG`
5. `Eirola_Nevanlinna`

The driver is useful because it shows:

- how to declare the nonlinear system and Jacobian as lambdas
- how to build the solver
- how to switch Jacobian strategies on an existing solver
- how to consume the returned `NewtonResult`

### `Makefile` and `Makefile.inc`

These files describe how the folder is built.

The Makefile can create:

- `libNewton.a`
- `libNewton.so`
- `main_Newton`

It reuses common variables from the repository-level build setup and supports:

- debug or release compilation
- static or dynamic library builds
- installation into the shared PACS include/lib directories

### `NewtonSolver.tex`

This is a companion document for the example.

Its role is to explain:

- the mathematical problem
- the Newton and quasi-Newton logic
- the role of backtracking
- the class design choices

It is not required for compilation, but it is useful for understanding the
motivation behind the code.

## Important Design Choices

### 1. Strategy Through Polymorphism

The solver delegates the local correction step to a polymorphic Jacobian
object. This is a classic strategy-pattern style separation:

- `Newton` controls the nonlinear iteration
- the Jacobian hierarchy controls how the correction is computed

This avoids mixing algorithmic responsibilities.

### 2. Traits For Type Uniformity

All solver and Jacobian classes inherit or reuse `NewtonTraits`, so they agree
on vector and matrix types without repeating definitions in multiple places.

### 3. Aggregates For Options And Results

Using `NewtonOptions` and `NewtonResult` keeps the API stable and readable.
Adding a new option or a new diagnostic does not require redesigning the whole
interface.

### 4. Callback For Extensibility

The callback is a simple but effective extension point. It avoids polluting the
core algorithm with logging or instrumentation code while still allowing those
features.

## Current Limitations And Risks

The code works on the provided example, but a few caveats are important.

### Backtracking Step Length Accounting

In `Newton.cpp`, `currentStepLength` is computed from `delta.norm()` before
backtracking damping is applied. If the accepted step is `lambda * delta` with
`lambda < 1`, then the stored step length is larger than the true accepted
step.

Consequences:

- printed diagnostics may be misleading
- convergence may be declared later than necessary

### Restart Semantics In Quasi-Newton Classes

`BroydenB`, `BroydenG`, and `Eirola_Nevanlinna` expose `setB()` with a default
empty matrix, but the method also forces `firstTime = false`.

That means:

- calling `setB()` without a valid matrix can disable safe initialization
- a subsequent call to `solve()` may use inconsistent history

So in practice `setB()` should be used only with a valid initial inverse
Jacobian matrix.

### Dense Linear Algebra Only

The current implementation assumes dense matrices and uses dense LU factorization.
That is appropriate for small and medium examples, but not for large sparse
nonlinear systems.

### Pointer Lifetime In `JacobianBase`

Some Jacobian classes depend on a stored pointer to the nonlinear system. The
current usage pattern in `Newton` is safe, but standalone use requires
discipline: the nonlinear system must outlive the Jacobian object that points
to it.

## Intended Use

This module is well suited for:

- teaching Newton and quasi-Newton design patterns
- experimenting with alternative Jacobian approximations
- small dense nonlinear systems
- examples where runtime flexibility matters more than peak performance

It is not yet a full industrial-strength nonlinear solver framework, but it is
a clean base for extensions such as:

- relative stopping criteria
- configurable linear solvers
- sparse Jacobians
- line search variants
- trust-region methods
- cloning support for polymorphic copy semantics

## Practical Reading Order

If someone is new to the folder, a good order is:

1. `README.md`
2. `NewtonTraits.hpp`
3. `NewtonMethodsSupport.hpp`
4. `Jacobian.hpp`
5. `Newton.hpp`
6. `Newton.cpp`
7. `main_Newton.cpp`
8. `NewtonSolver.tex`

That sequence starts from the public concepts, then moves to the abstraction
layer, and only afterward dives into implementation details.
