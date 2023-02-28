# Round-off error due to cancelation #

This is another example of round-off error accumulation due to
cancellation. Using the common formula for the root of a quadratic
polynomial, expecially when using `float` (4 bytes precision) floating
point type, In the case where |b|>>|ac| you have a cancelation error for one of the roots, due to the
subtraction of two quantities of very similar magnitude.

The code then implements a different formula for the computation of the roots,
which is more stable with respect to round-off errors.

Typing `make` will produce two executables `main_float` and `main_double`,
the first uses floats to store variables. Note the use of `-DReal=double|float` option to change the type in the program, 
the code indeed uses a preprocessor macro variable.

**Note:** I have implemented the code using `std::complex`, so this is
also an example of complex numbers in C++

# What do you learn with this code #
- That the use of less standard numerical algorithms may be necessary if you wish to control roudoff error in particular situations;
- The use of CPP macro to select different compilation options;
- The use od `std::complex`.
