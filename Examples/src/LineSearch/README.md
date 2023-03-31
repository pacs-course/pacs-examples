# A tool for line search methods for unconstrained and box-constrained optimization #

This set of files introduce a general framework for line search methods for the minimization of a function
f: Rn -> R.

We recall that a line search method follows this general algorithm. For a given X0

- Compute a descent direction d
- Compute the "step lenght" alpha via backtracking: you make sure that f(xk+alpha d) is sufficiently smaller than f(xk)
- Adjourn x: x_{k+1} = xk + alpha d
- Test for convergence

From this algorithm description if it clear that that key component is the search of a good descent direction. In this respect many techniques are possible: 
gradient method, quasi-Newton (like BFGS), Barzilai-Borwain, non linear conjugate gradient, gradient with moment....just to mention few.

So the idea was to define a general framework for the computation of the descent direction, in the form of an abstract class, 
and implement a few methods via inheritance and polymorphism.

We have (so far) implemented

- Gradient scheme;
- classic BFGS that approximates the Hessian;
- BFGS that approximates the inverse of the Hessian directly;
- Borzilai-Borwain method
- Newton method

This version implements also the projected-gradient and projected-Newton methods for 
box constrained problems (not for the other methods).

We give a brief description of the main files

- *LineSearch_traits.hpp* contains the main types used throughout the utilities, to maintain concistency of types;
- *LineSearch_options.hpp* The structure with the options for the backtracking (it should be renamed to backtracking_options.hpp);
- *Optimization_options.hpp* The options for the line search algorithm;
- *DescentDirectionBase.hpp* The base class for descent direction computation;
- *DescentDirections.hpp* The implemented conclrete classes for computing descent directions;
- *LineSearchSolver.hpp* The class with the line search algorithm;
- *GradientFiniteDifference.hpp* If you are lazy and you do not want to compute the gradient by hand.

In *main_linesearch.cpp* you have an example of use, with different options commented.

##What do I learn here?#
- A rather complete code (however, read the notes) that implements one of the basic technique for optimization.
- The use of polymorphism to select different implementations of the descent direction computation
- The use of aggregates to store the parameters of the algorithm in a single place


##Notes##

The object containing the concrete implementation of the descent direction is stored in the LineSearchSolver class as a unique pointer to the base class. Since I have not implemented
cloning it means that the object has to be passed as unique pointer by **moving** the pointer. Moreover, as a consequence LineSearchSolver class is not copy-constructible nor copy-assigneable. But I think it is
not a great limitation.

Many improvement can be made. For instance: 
- reading options from a text (maybe GetPot) file; 
- adding more methods for the computation of the descent direction;
- extend it to more general contraints;
- implement the 2nd Wolfe condition
- make it a library.


 



