# A tool for fixed point iterations #

We define a class template `FixedPointIteration` that implements the simple fixed point iteration 

``` C++
x(k+1)=phi(x(k)) 
```
with the following features

* The type for argument and return of `phi` are user defined and set in FixedPointTraits. Some
  possible choices are predefined to store the argument in a
  `std::vector` or in a Eigen vector, but the user may make other choices.
* In `VectorInterface.hpp` we implement some adaptors to be able to operate seamlessly on `std::vector` and Eigen vectors. For `std::vector` I have chosen
to use some standard algorithms that allow a parallel version, even if in this version I do not use the parallel capability.
* We can add an accelerator, and some are provided in `Accelerators.hpp`. 
    An accelerator takes as input the last iterate and returns a corrected value which hopefully
    provides a better convergent sequence. I have made the choice to implement the accelerators as a policy, so that they can be developed independently of the main class. Moreover, I decided to agglomerate the iterator function in the accelerator, to save memory in case phi is a callable object of big size. With agglomeration the accelerator and the main class share the same memory for the iterator function.  
* Test of convergence based on the distance of two consecutive iterates (which is classical for fixed point iterations).
* I have used an aggregate (a `struct` called `FixedPointOptions`) to store the options, which have a default value. You may change the options by passing a modified
`FixedPointOption` to the the solver class. A possibility is to make this struct internal to the `FixedPointIteration` class. Indeed, it is used only in conjunction with the use of a `FixedPointIteration`.  
* If you want to have a more verbose output compile with

```
make VERBOSE=yes
```

## Accelerators implemented ##
- `NoAccelerator`: the simplest accelerator, it does nothing.
- `ASecant`: It is two level Anderson acceleration, that may be considered as the multidimensional estension of the secant method for finding the zero of `f(x)=x-\phi(x)`. A good reference of the technique is found in  *H. Fang, Y. Saad, Two classes of multisecant methods for nonlinear acceleration, Numerical Linear Algebra with Applications 16 (2009) 197–221.*
- `Anderson`. It implements Anderson acceleration. A good reference is *H.F. Walker, N. Peng, Anderson Acceleration for Fixed-Point Iterations, SIAM J. Numer. Anal., 49(4), 1715-1735, 2011*. The algorithm is also described in the previously cited paper by H. Fang and Y. Saad. Anderson acceleration requires to indicate the number of previous iterates to be used for the acceleration. The default value is 10, but you may change it by passing a different value in the constructor of the accelerator. The selection of this parameter is critical. Higher values increase the computational cost of each iteration, but it may reduce the number of iterations needed to converge, even if this is not necessarily true. So, you may need to experiment with different values.


# What do I learn from this example? #

- The use of adapters to make your code more general, in particular to enable it to interface with objects with different public interfaces
- The strange way of passing Eigen matrices as function parameters, due to the adoption of expression templates by Eigen.
- The use of preprocessor macros to selectively compile part of the code, in this case in order to have a more verbose output
- The use of some standard algorithms
- The use of traits to centralize the declaration of types used throughout the code and to enable the selection among different possible choices.
- The use of policies (the accelerators) to implement part of an algorithm with a set of classes that can be developed separately. it is an example of the Bridge design pattern (here implemented via templates).
- The use of user defined traits to check if an enumerator has a certain value. This is useful to check if we are using EIGEN vectors as argument for the iterator function, since the given implementation of the Anderson acceleration works only in that case.

