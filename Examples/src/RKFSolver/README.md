#Function for explicit Runge Kutta (Fehlberg type) adaptive time integration#

For scalar or vector ODE of the form dy/dt = f(y,t)

You have two version of the code. One in rk45.[hc]pp is an simple version
where you find the classic RK45 scheme (Runge Kutta embedded Order 4 and
5) for scalar problems. It is implemented as a function and the coefficients are
hard coded. It is used by other examples and is rather efficient.

A new version is contained in RKF.[hc]pp (and includes
`Butcher.[hc]pp)`. It is a template class that accepts as template
parameter a Butcher array. The structure of the Butcher array is given
in the template struct `ButcherArray`, templated with the total number
of stages of the method. This allows to implement different methods,
indeed I have implemented both `RK45` and `RK23`.

Instruction (to install the old version as a library, for other Examples)

    make alllibs [DEBUG=no] (makes static and dynamic library)
    make install 

`make exec` will also create the executable with a test.

The test solves a simple scalar ode using both the function and the template class version, and a vectorial problem (Van der Pol equations). The results are printes in the files

* `result.dat`  Produced by the code implemented as a function;
* `result2.dat` Produced by the code implemented as an instance of the RKF class;
* `resultVDP.dat` Solution of the vectorial problem produced by an instance of the RKF class.

Some scripts are provided that use gnuplot to show the results

**Note** The techinque for error estimation and time step control is
rather pesimistic, so usually is over-cautelative. It controls the
error at the final time step with a procedure that uses the estimeate
given by the embedded RK scheme to control that the local truncation
error is less than the tolerace divided the fraction of the current
time step and the length of the desired integration interval. This can
be very restictive, you can change to more relaxed (but maybe less
accurate) techniques.

**Important note** Is the problemi is stiff the methods here provided
are inadequate. In a stiff problem the method will fail becouse it set
the time step very small for stability reasons. You may guess that
your problem may be stiff when it fails because it reaces the maximal
number of time steps. You may try to increase that number (it is the
last argument in the call operator), but if the probelm is very stiff
you need to use A-stable (and thus implicit) methods. You may refer to
more advanced libraries for this purpose.
