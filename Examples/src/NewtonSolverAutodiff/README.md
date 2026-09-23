# Newton solver with automatic differentiation

This example follows the `NewtonSolver` API (`Newton`, `NewtonVerbose`,
`NewtonOptions`, and `NewtonResult`) but obtains the Jacobian with the bundled
forward-mode `autodiff` library instead of finite differences or a hand-written
Jacobian.

Write the nonlinear residual as a function from
`apsc::NewtonTraits::AutoDiffArgumentType` (`autodiff::VectorXreal`) to the same
type, using autodiff-compatible scalar operations. `Newton::solve` still accepts
and returns `Eigen::VectorXd`.

Build and run:

```sh
make
./main_NewtonAutodiff
```

## Note ## 
In principle it could be possible to refactor the code in NewtonSOlver to accept also Jacobian computed with automatic differentiation. However, the refactoring woudl complicate the structure considerably, so I decided to have a separate version instead.
Hwever, I have maintained the sam eintervace, so for the user it is enough to use the namespace newton_ad or apsc::netwon_ad instead of apsc in ordert to use the version with AD. A parte the specification of the non linear system whch requires new the use of the special typess defined in autodiff.

# What do I learn here: a use of autodiff for automatic differentiation