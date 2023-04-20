# Tools for Newton and quasi-Newton methods for the zero of a system of non linear equations #

We present here a set of tools to implement generic Newton or quasi newton methods. They are based on the eigen library and 
on the following main classes

* `JacobianBase` a base class with implements the action of a "quasi-Jacobian", the user may choose among `FullJacobian`, where the actual Jacobian (or an approximation of it) must be specified by the user, `Discretejacobian`, that computes an approximate Jacobian via finite differences, `IdentityJacobian`, that just implements a scaled identity map, `BroydenB`, that implements the "bad" Broyden iteration and the `BroidenG` that implements the classic Broyden.
* `Newton` A class that given the non linear functions and a `JacobianBase` will perform the Newton iterations.

We have also:

* `NewtonTrats` Contains the definition of the types used by the main classes, to guarantee uniformity;
* `NewtonOptions` and `NewtonResults`, structs that encapsulate the options and the result;
* `JacobianFactory` to generate a concrete class of `JacobianBase` family on the fly.

**Note** the use of the virtual function member `callback` in `Newton` allows the user to get hold of the state at each iteration. See `NewtonVerbose` in `Newton.hpp`, to see a possible use of it.

The file `main_Newton.cpp` contains a test.

If you launch `pdflatex NewtonSolver.tex` you get `NewtonSolver.pdf` with some more details on the procedure.

# What do I learn here? #
- A rather complete example where we use traits, aggregates to collect strictly related variables, polymorphism, object factory.
- The use of a callback()
