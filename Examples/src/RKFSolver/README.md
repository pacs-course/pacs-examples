# Runge Kutta (Fehlberg type) adaptive time integration #

**New version July 2021** Extended to treat also DIRK (Diagonally
Implicit) schemes. Only for vectorial problem (but you can still solve
scalar problems if you set the dimension of the vector to 1). This new
version needs the updated version of the `Newton` class, available in
`NewtonSolver` (follow the instruction in that folder to install it).



The explicit schemes have been implemented for scalar, vector or matrix ODE of the form dy/dt = f(y,t). The implicit schemes are implemented only for vectorial problems dy/dt = f(y,t). Only DIRK (Diagonally Implicit Runge Kutta) schemes are supported.

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
rather pessimistic, so usually is over-cautelative. It controls the
error at the final time step with a procedure that uses the estimate
given by the embedded RK scheme to control that the local truncation
error is less than the tolerace divided the fraction of the current
time step and the length of the desired integration interval. 
This can be very restictive, you can change to more relaxed (but maybe less accurate) techniques.

**Important note** Is the problem is stiff the explicit methods are inadequate.
You have to use a DIRK scheme

**Some Explanations** are included in the LaTex file you find in this folder.

## Constexpr Butcher arrays ##
You may note that the Butcher arrays have a *constexpr constructor*.
```c++
constexpr ButcherArray(Atable const &a, std::array<double, NSTAGES> const &b1,
                         std::array<double, NSTAGES> const &b2, int ord)
    : A{a}, b1{b1}, b2{b2}, c{}, order{ord}
  {
    std::transform(A.begin(), A.end(), c.begin(), [](auto const &row) {
      return std::accumulate(row.begin(), row.end(), 0.0);
    });
  }
  set_implicit<NSTAGES>();

```
This is a C++17 feature that allows to create a Butcher array at compile time. This is useful to create a Butcher array that is a constant expression, and can be used to create a template class that accepts a Butcher array as a template parameter and may allow the compiler to speedup some calculations. The Butcher arrays are indeed defined as `constexpr` variables in the `Butcher.hpp` file. 
This is possible because they have a constexpr constructor. I needed to define the constructor and not use the synthetic one since I need to fill the vector `c` with the sum of the elements of each row of the matrix `A`. This is done in the body of the constructor. Note that I am using two standard algorithms `std::transform` and `std::accumulate` to do this and not a for loop. The reason is that using a for loop is forbidden in a constexpr function. While, since C++20, those two algorithms are constexpr functions, so I can use them in a constexpr constructor.

`set_implicit()` is a constexpr function member that sets a flag to true if the scheme is implicit. This is used to decide if the Newton method should be used to solve the implicit equation. 
This function recognises that a scheme is an implicit scheme by checking if one of the diagonal elements of A is different from zero. This is a simple way to check if the scheme is implicit. It assumes thet the implicit scheme has a diagonal element of the A array different from zero. This is true for all the DIRK schemes, the only supported here, *but it is not true for all the implicit RK schemes*. 

**Another warning:** at the moment DIRK schemes are implemented only for vectorial problems. So if you have a scalar problem and you want to treat it by using a DIRK scheme, you have to set the dimension of the vector to 1. This is not a limitation of the method, but of the implementation.

- By typing `make exec` you will run the test program that solves a simple scalar ode using both the function and the template class version, and a vectorial problem (Van der Pol equations). The results are printes in the files

- By typing `pdflatex RKExplanation.tex` you will compile the LaTex file that gives more datails on the scheme.

# What do I learn here? #
- A way to build a generic code for both explicit and implicit formulations
- The use of strategy pattern and traits
- The use of constexpr constructors to allow the creation of constant expressions