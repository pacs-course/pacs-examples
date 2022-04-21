#Floating point exceptions#

When you perform an invalid floating point operation (the square root
of a negative number) or you have overflows/underflows, the execution
does not stop. Yet in many architectures a flag is raised to indicate
that a **floating point exception** has occurred. You may test if the
flag is on and take appropriete action, if you wish. But it require
some work.

This directory contains two utilities

* `trapfpe.hpp` a header only utility that you should include in your main source file if you want to be able to activate "abort on major fp exceptions".  To activate this feature you have to compile your code with the cpp option -DFPE_ABORT. If not, nothing happens and you have the default behaviour in case of floating point exceptions.  With major floating point exceptions I mean: invalid operation, divide by zero, overflow.

* `fpeUtilities.[hc]pp` the sources of a little library that introduces some exception classes inherited from std::runtime_error and a function that tests if major fp exception has been raised and in that case launches the relevant exception, or returns a boolean equal to true. To use this utility you need to link with the libfpe library created in this example. 

A test is available, `main_fpe.cpp`, to test the utilities.

TO COMPILE AND INSTALL

```
make static; make exec  # creates static lib and executable

make dynamic; make exec # creates dynamic lib and executable

make install # installs libs and header in main lib and include dirs

make exec; // makes just the executable with the test

```

If you specify `FPE_ABORT=yes`, for instance: `make exec FPE_ABORT=yes` you
are activating "abort on fpe" in the test example.

Remember that you have to make 

```
make install 
```
to install in the correct directory all headers and library
`libfpe.[a|so]` containing the compiled fpe Utility for use in other
examples.
