#Template function overloading#

Try to comment some of the definitions in `functions.hpp/cpp` and `functions2.hpp` and see what happens.

In particular, if you comment out `void funct(T & x)` and `void
funct(T const & x)` in `functions2.hpp`, the function `void
funct(T&&)` will exploit the "universal reference" technique and it
can bind to any type of argument. But then, you have to use
`std::forward` to separate rvalues from lvalues arguments.

# What do I learn here #
* A revision of function overloading and reference bindings.
* That it is better not to mix free functions and template functions with the same name.

