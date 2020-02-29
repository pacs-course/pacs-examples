#A tool for fixed point iterations#

We define a class template `FixedPointIteration` that implements the simple fixed point iteration 

``` C++
x(k+1)=phi(x(k)) 
```
with the following features

* The type for argument and return of `phi` are user defined. Some
  possible choices are predefined to store the argument in a
  `std::vector` or in and `Eigen::Vector`, but the user may make other
  choices, the important thing is the the type compies with the
  following interface of `std::vector`: addressing operator `[]` and
  method `size()`.
* We can add an accelerator, and some are provicede in `Accelerators.hpp`. 
    An accelerator takes as input the last iterate and returns a corrected value which hopefully
    provides a better convergent sequence.
* Test of convergence based on the distance of two consecutive iterates.
