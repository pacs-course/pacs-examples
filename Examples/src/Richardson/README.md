# Richardson Extrapolation #

This example presnts a possible implementation of Richardon extrapolation. In the file `Richarsdon.tex` you have a description of the technique. You can produce the pdf  with `pfdlatex Richardson.tex` or with `make Richarsdon.pdf`.

It contains also a class for the solution of scalar ordinary differential equation with the Crank-Nicolson method, used for the example. The code uses some utilities in `Utilities` and `LinearAlgebra/Utilities`. Make sure you go into those directory and do `make install`. 

Notable things:
  * the use of the function type as template parameter, defaulted to `std::function`; 
  * The use of lambda expressions to define the forcing term for the ODE end to trasnform the call to Crank-Nicolson code inte a function suitable for the `Richardson` class.
  
## What do I learn here? ##

- An interesting acceleration method;
- An example of template class and functions;
- Use of lambda expressions.

