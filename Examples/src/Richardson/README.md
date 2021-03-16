# Richardson Extrapolation #

This example presnts a possible class to implement Richardon extrapolation. In the file `Richarsdon.tex` you have a description of the technique. You can produce the pdf  with `pfdlatex Richardson.tex` or with `make Richarsdon.pdf`.

It contains also a class for the solution of scalar ordinary differential equation with the Crank-Nicolson method, used for the example. The code uses some utilities in `Utilitilities` and `LinearAlgebra/Utilities`. Make usre you go into those directory and do `make install`. 

Notable things:
  * the use of the function type ad template parameter, defaulted to
  `std::function` 
  * The use of lambda expressions to define the forcing term for the ODE and to trasnform the call to Crank-Nicolson code inte a function suitable for the `RischardSon` class.
  
## What do I learn here? ##

- An intersiting acceleration methods
- An example of template class and functions
- Use of lambda expressions

