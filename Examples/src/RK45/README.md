# THIS IS AN OLD VERSION. LOOK AT THE ONE IN RKFSOLVER DIRECTORY INSTEAD #






## Function for Runge Kutta adaptive time integration ##

For scalar ODE of the form dy/dt = f(y,t)
(extension to vectorial problems is not difficult)

You have two version of the code. One in rk45.[hc]pp is an old version
where I have implented a RK45 code (Runge Kutta embedded Order 4 and
5). It is rather simple but not very flexible. The coefficients are
hard coded and it implements only the RK45 rule. It has been left as
an example and because it is used by other examples.

A new version is contained in RKF.[hc]pp (and includes
`Butcher.[hc]pp)`. It is a template class that accepts as template
parameter a Butcher array. The structure of the Butcher array is given
in the template struct ButcherArray, templated with the total number
of stages of the method. This allows to implement different methods,
indeed I have implemented both `RK45` and `RK23`.

Instruction (to install the old version as a library, for other Examples)

    make alllibs [DEBUG=no] (makes static and dynamic library)
    make install 

`make alllibs` will also create the executable with a test.

`bash plot.sh` tu use gnuplot to show the result

**NOTE** This is a relatively old implementation. It has been replaced by `RKFSolver`.
