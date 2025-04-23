# This directory contains a series of examples of optimization and profiling #

The makefile is specially written so it compiles and executes some of examples. We need some of the tools of the `Utility` folder. So, if you have not done it yet, 
you to go in the directory `Utilities/` and follow the instruction to install the utilities.

This  set of stand-alone examples are meant to explain some tools for profiling, coverage, and also on how to asserts in a smarter way than the classic `assert`. 
I recall that `assert` is a test made **run time** (do not confuse it with `static_assert`, which works at compile time). Normally, asserts are used to
control code execution during the development phase and catch possible bugs. They are disactivated by the `NDEBUG` cpp macro. The basic `assert` utility of C++ (inherited from C) is in fact a preprocessor macro that just stops the program if a condition is not met. The asserts proposed in `extendedAssert.hpp` (contained in `Utility/`)  are build on top 
of the basic assert facility, but they provide the possibility of adding a user-defined message and a finer control on activation/deactivation.

* `make esempioExtAss`: produces an example of use of the extendedAssert
     facility provided . The programs `esempioExtAss_debug` and `esempioExtAss_release` are generated one with the assertions
     activated and the other not. The file `Rational.dat` is read and contains a list of rationals in the form numerator denominator.
     It may be modified to assess the activation of the assertions (try a zero denominator).

* `make optimize` shows the effect of different optimization level on the CPU time to solve a simple fem program. In the file dati you should write the number of elements you want. Suggested: 300.

* `make debug`. Produces two executable, `esempio0_debug` and
     `esempio1_debug`. The latter contains an error. You should try to
     use the debugger. Use `ulimit -c unlimited` to do static debugging (see note below).

* `make leak`. An example of use of valgrind to find a memory leak. You should have [valgrind](https://valgrind.org/) installed.


* `make profile`. An example of use of `gprof` profiler. The `--annotated-source` option is  working only if the compiler option `-g` is activated. But better use `gcov` to have the 
info provided by `--annotated-source`, so I am not using it.

* `make massif`. An example of use of `massif` tool of `valgrind`  to analyze memory usage.

* `make aliasing`: a code that may produce wrong results because of aliasing

* `make coverage` : use of `gcov` to to the coverage of a program. It produces a lot
     of files, but you can clean the folder up using `make clean`

* `make lcoverage` : like `coverage` but uses `lcov` and `genhtml` to have nicer result in html format!  You must have them installed!
     
* `namemangling,cpp` does not create any executable. is only to test namemangling. You should do `g++ -c namemangling.cpp`
    and run `nm namemangling.o` or `nm --demangle namemangling.o` to see the difference.

`rational.[h|c]pp` and `GetPot` are only support utilities used in some of the previous examples. 

# What do I learn here? #
- Different tools and utilities for debugging and profiling

## A note on core dump
Normally core dumping is suppressed to avoid having big files around when not needed. The `ulimit -c unlimited` command
removes this restriction. However **on ubuntu systems** this is not enogh if you have *apport* active. Apport is a daemon that automatically reports to Canonical possible system bugs and handles the core, which is then are no more saved in the working directory. Therefore, if you want to use static debugging on the given example you have also to stop apport with `sudo systemctl stop apport` or (alternatively) `sudo service apport stop`. 
These commands suspend apport until you run them again with `stop` replaced by `start`, or at the next reboot. If you want to disable apport permanently, look at the documentation on line.