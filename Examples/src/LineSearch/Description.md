# Algorithms and architecture

## Problem and iteration

`LinearSearchSolver` addresses unconstrained problems

\[
  \min_{x \in \mathbb{R}^n} f(x)
\]

or boxeed-contrained problems where

\[
  x\in \mathcal B = \{y : l_i \leq y_i \leq y_i,\ i=1,\ldots,n\}.
\]

At iteration \(k\), the solver stores \(x_k, f_k, g_k, H_k\), obtains a
direction \(d_k\) from the selected strategy, and forms a trial point

\[
  x_{k+1}^{\mathrm{trial}} = x_k + \alpha d_k.
\]

For an unbounded problem, the trial point is used directly. For a bounded
problem it is clamped component-wise:

\[
  x_{k+1}^{\mathrm{trial}} = \Pi_{\mathcal B}(x_k + \alpha d_k),
  \qquad
  [\Pi_{\mathcal B}(z)]_i = \min(\max(z_i,l_i),u_i).
\]

The process stops when at least one of the gradient-norm, step-length, or
objective-change tests fails to exceed its tolerance, or when the configured
maximum number of iterations is reached. Tolerances and iteration limits live
in `OptimizationOptions`.

## Step-length selection

`LinearSearchSolver::backtrack` uses geometric backtracking with the Armijo
sufficient-decrease condition (the first Wolfe condition):

\[
 f(x_k + \alpha d_k) \leq f_k + c_1\alpha g_k^T d_k.
\]

It starts from `LineSearchOptions::initialStep`, reduces the step by
`stepSizeDecrementFactor`, and performs at most `maxIter` reductions. The
configuration member `secondWolfConditionFactor` is retained for a future
extension, but the curvature/second-Wolfe condition is **not implemented**.

If a direction is not a descent direction, the current implementation replaces
it with the negative gradient before starting backtracking. A near-zero
direction is accepted as a zero step.

## Direction strategies

All direction classes implement `DescentDirectionBase::operator()`, taking an
`OptimizationCurrentValues` object and returning a vector. The factory maps
the names below to their implementations.

| Factory name | Direction | Notes |
| --- | --- | --- |
| `GradientDirection` | \(-g_k\) | Steepest descent. |
| `NewtonDirection` | solves \(H_k d_k=-g_k\) | Uses an LLT factorization for unbounded problems; requires a suitable Hessian. |
| `BFGSDirection` | solves \(B_kd_k=-g_k\) | Maintains a BFGS approximation \(B_k\) to the Hessian. |
| `BFGSIDirection` | \(-H_k^{\mathrm{inv}}g_k\) | Maintains a BFGS approximation to the inverse Hessian. |
| `BBDirection` | \(-\alpha_k g_k\) | Uses a blend of the two Barzilai--Borwein scalar steps. |
| `CGDirection` | Polak--Ribiere Conjugate Gradient recurrence | Restarts to \(-g_k\) when the recurrence is no longer descending. |

The BFGS, inverse-BFGS, Barzilai--Borwein, and conjugate-gradient strategies
retain state from the previous iterate. The solver calls `reset()` when a new
initial point is set, which is essential before reusing one of these objects.
BFGS updates are guarded by a curvature test so that an update is only applied
when \(y_k^Ts_k\) is sufficiently positive.

## Derivatives

The problem is supplied through `OptimizationData`:

- `costFunction` and `gradient` are required before calling `setInitialPoint`;
- `hessian` is optional in the data structure but is needed by
  `NewtonDirection`.

Three ways of obtaining derivatives are illustrated:

1. Define analytic gradient and Hessian callables directly.
2. Use `GradientFiniteDifference<FDT>` for forward, backward, or centered
   finite-difference gradients. Its step size scales with the norm of the
   evaluation point.
3. Use `GradientsAD` with a cost function written for `autodiff::VectorXvar`.
   It exposes ordinary-double callables suitable for `OptimizationData` and
   uses reverse-mode autodiff for gradients and Hessians. `test_gradientAD.cpp`
   demonstrates this wrapper.

`GradientsAD::evaluateAll` can compute a cost, gradient, and Hessian together.
In the normal solver loop, however, the three callables are invoked separately;
this favors a clear interface over avoiding repeated autodiff evaluations.

## Architecture and programming choices

### Common types and data aggregates

`LineSearch_traits` centralizes the scalar, Eigen vector/matrix, and callable
types. This avoids mismatched types between the solver, derivative providers,
and strategies, and makes a future numerical-backend change localized.

`OptimizationData` holds immutable-by-convention problem definitions:
cost, gradient, optional Hessian, dimension, and optional lower/upper bounds.
`OptimizationCurrentValues` carries the state observed by a direction strategy:
the current point, cost, gradient, Hessian, and bound data. Algorithm and
backtracking parameters are kept separately in `OptimizationOptions` and
`LineSearchOptions`; both can be read from JSON files.

### Strategy polymorphism and ownership

The direction computation is a strategy object behind the abstract
`DescentDirectionBase` interface. `LinearSearchSolver` owns it through a
`std::unique_ptr`, so the concrete method can be selected at run time without
making the solver itself a template. The `clone()` operation is available on
the strategies to support deep-copy designs when needed, although the solver's
normal construction path transfers ownership with `std::move`.

`DescentDirectionFactory` registers the standard strategies under stable string
names. This enables selecting the algorithm from `linesearch_options.json`
without changing the driver. A custom method is added by deriving from
`DescentDirectionBase`, implementing `operator()`, `reset()` if it stores
history, `clone()`, and registering a constructor in `loadDirections()`.

## Box constraints: current behavior and limitations

Calling `setBounds(data, lower, upper)` marks a problem as bounded. The initial
point must already satisfy the bounds; otherwise `setInitialPoint` throws. Each
trial point in backtracking is projected with `std::clamp`, so accepted points
remain feasible.

For a projected trial point, the Armijo test uses a modified directional
derivative when the trial is on a boundary. `NewtonDirection` also identifies
active components and removes their rows and columns from its inverse-Hessian
calculation, returning zero motion if all components are active.

These facilities are intentionally limited and should be viewed as a teaching
example, not a complete active-set or projected-gradient implementation:

- projection is performed on trial points; the generic gradient, quasi-Newton,
  BB, and CG strategies do not explicitly compute a projected search direction;
- the outer stopping test uses the norm of the ordinary gradient rather than a
  projected-gradient/KKT residual, which can be nonzero at a correct
  boundary solution;
- equality tests against bounds are used to identify active components, so
  numerical tolerances near a bound are not yet modeled;
- general equality, inequality, and nonlinear constraints are outside the
  scope of this code.

A robust constrained extension would use a projected-gradient stationarity
test, a tolerance-aware active set, feasible directions for every strategy,
and preferably a dedicated bound-constrained method such as L-BFGS-B or a
projected/trust-region algorithm.

## Further extensions

Natural next steps include strong-Wolfe line searches, safeguarded Newton steps
for indefinite Hessians, L-BFGS, preconditioning, better termination reports,
and a clearer separation between evaluating the objective/derivatives and
updating the optimization state.
