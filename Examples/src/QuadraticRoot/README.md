#Round-off error due to cancelation#

This is an example of round-off error accumulation due to
cancellation. Using the common formula for the root of a quadratic
polynomial, expecially when using float (4 bytes precision) for floating
point type, in the case where b>>ac you have a cancelation error due to the
subtraction of two quantities of very similar magnitude.

The code then implements a different formula for the computation of the roots,
which is more stable with respect to round-off errors.

Typing `make` will produce two executables main_float and main_double,
the first uses floats to store variables. Note the use of `-D` option to change the type in the program manually, the code indeed uses a preprocessor macro variable.

**Note:** I have implemented the code using `std::complex`, so this is
also an example of complex numbers in C++

