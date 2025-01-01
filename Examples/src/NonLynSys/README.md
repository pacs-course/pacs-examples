# A class for non systems of function s#
This is an example of the flexibility of C++. I have built a class to store a vector of functions which may
be rather general.

The idea is to create a class that stores a vector of functions, and that may be called as a whole or singularly.

We have here a class `NonLinSys` that stores a vector of functions. The functions are stored as `std::function` objects. Thanks to the use of `std::function` we may store any kind of function, including lambda expressions. The class is designed to store functions that take a single argument and return a single value. The argument may be a vector or a scalar. The return value may be a vector or a scalar, depending on how the traits are defined. Using traits I can esily modify the type of the argument and the return value. For instance I may decide to use vectors of the Eigen library instead of standard vectors. some example of traits are defined in the file `NonLinSysTraits.hpp`.

The system of functions may be called singularly or as a whole. I have also designed a function factory, that use the `NonLinSys` class
to store functions that may be recalled on demand.

## Parallel implementation ##
When you call the system as a whole the call to each function is embarassingly parallel. 
- If you compile with `-DPARALLESTD` the code will be parallelized using a standard algorithm. Remember to link the code with `-ltbb`.
- Otherwise,  I have used openMP to parallelize the call to each function. To activate it you have to compile with `-fopenmp`.

# What do I get here? #
- A use of `for_each` as a parallel algorithm
- Another usage of `std::function` and lambda expressions
- The use of traits and an example of template template parameters
- An example of a function factory (a particular object factory)
- A simple use of parallelization using openMP
