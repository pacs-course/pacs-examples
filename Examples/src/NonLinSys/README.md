# A class for non-linear systems of functions
This is an example of the flexibility of C++. I have built a class to store a vector of functions that can be used
 1. as a system of functions
 2. as a single function

Moreover, the argument and return type of the individual functions in the system may be specified
via traits. The class is designed to be used in parallel, and I have implemented two parallelization strategies.

The idea is to create a class that stores a vector of functions, and that may be called as a whole or individually.

We have here a class `NonLinSys` that stores a vector of functions. The functions are stored as `std::function` objects. Thanks to the use of `std::function` we may store any kind of function, including lambda expressions. The class is designed to store functions that take a single argument, and return a single value. The argument may be a vector or a scalar. The return value may be a vector or a scalar, depending on how the traits are defined. Using traits, I can easily modify the type of the argument and the return value. For instance, I may decide to use vectors of the Eigen library instead of standard vectors. Some examples of traits are defined in the file `NonLinSysTraits.hpp`.

The system of functions may be called individually or as a whole. If you call it as a whole, it returns a vector of values, one for each function. If you call it individually, it returns the value of the function at the given index.

To call the ith function for the argument `x` you may use the following syntax:
```cpp
apsc::NonLinSys sys;
std::vector<double> x;
double y = sys.getFunction(i)(x);
```
to callit as a whole you may use the following syntax:
```cpp
apsc::NonLinSys sys;
std::vector<double> x;
auto y = sys(x);
```
Here I have indicated the default case where the argument is a standard vector of doubles and the output a standard vector of double. The traits are defined in the file `NonLinSysTraits.hpp`, and can be changed by setting the corresponding tempalte argument:

```cpp

apsc::NonLinSys<double,  apsc::NonLinSysTraitsScalarTraits> system;
```
define a system where the argument is a scalar (double) and return type (when called gobally with ()) is a vector of doubles.

```cpp

I have also designed a function factory, that uses the `NonLinSys` class
to store functions that may be recalled on demand.

## Parallel implementation ##
When you call the system as a whole the call to each function is embarassingly parallel. 
- If you do `make parallel` you activate c++ parallelization using standard algorithms. You neet to have the libtbb.so library installed
- If you do `make parallel_omp` you activate parallelization using openMP. 
- If you just do 'make' you do not activate parallelization.

Remember that you have to use the make option `-B` to force the recompilation of the code.

# What do I get here? #
- A use of `for_each` as a parallel algorithm
- Another usage of `std::function` and lambda expressions
- The use of traits and an example of template template parameters
- An example of a function factory (a particular object factory)
- A simple use of parallelization using openMP
