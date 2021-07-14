#Function for Runge Kutta (Fehlberg type) adaptive time integration#

**New version July 2021** Extended to treat also DIRK (Diagonally
Implicit) schemes. Only for vectorial problem (but you can still solve
scalar problems if you set the dimension of the vector to 1). This now
version needs the updated version of the `Newton` class, available in
`NewtonSolver` (follow the instruction in that folder to install it).



For scalar or vector or matrix ODE of the form dy/dt = f(y,t)

The main class is contained in RKF.[hc]pp (and includes
`Butcher.[hc]pp)`. It is a template class that accepts as template
parameter a Butcher array. The structure of the Butcher array is given
in the template struct `ButcherArray`, templated with the total number
of stages of the method. This allows to implement different methods,
indeed I have implemented  `RK45` and `RK23` and many others, and you can add more! 

Instruction (to install the old version as a library, for other Examples)

    make alllibs [DEBUG=no] (makes static and dynamic library)
    make install 

`make exec` will also create the executable with a test.

The test solves a simple scalar ode using both the function and the template class version, and a vectorial problem (Van der Pol equations). The results are printes in the files

* `result.dat`  Produced by the code implemented as a function;
* `resultVDP.dat` Solution of the vectorial problem produced by an instance of the RKF class.
* `resultstiff.dat` Solution of a stiff problem.

Some scripts are provided that use gnuplot to show the results

**Note** The techinque for error estimation and time step control is
rather pesimistic, so usually is over-cautelative. It controls the
error at the final time step with a procedure that uses the estimeate
given by the embedded RK scheme to control that the local truncation
error is less than the tolerace divided the fraction of the current
time step and the length of the desired integration interval. This can
be very restictive, you can change to more relaxed (but maybe less
accurate) techniques.

**Important note** Is the problem is stiff the explicit methods are inadequate.
You have to use a DIRK scheme

**Some Explanations** Are included in the aTex file you find in this folder.
