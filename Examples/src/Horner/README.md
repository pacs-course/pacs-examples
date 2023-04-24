# Computing a polynomial with Horner rule. #

Horner's rule is a fast and stable way of computing the value of a polynomial at a given point.  

## How to compile ##

You should first enter the directory Examples/src/Utilities and then run

````
make library
make install
````

Make sure that ``Makefile.inc`` in the PACS root directory is set correctly.

## Parallel version ##
This version of the example exploits also a parallel version of an algorithm
(``std::transform``) of the standard library. However the
implementation of concurrency is not yet fully supported by all
compilers and architecture. If you use gnu compilers, you must have
`g++` at least version 9. And, on intel or intel-compatible
architecture you need a recent version of the `tbb` libraries.

To have the system correctly set for the parallel version, you need to set up correctly
some variable in the **local** `Makefile.inc` file (or create corresponding environment variables)

* `mkTbbLib` should contain the directory where `libtbb.so`
resides. If you have a standard istallation you may put `/usr/lib`, If
you use the module system for the PACS course this value is set
through an environmental variable.

* `mkTbbLib` should contain the directory where the header files of
the `tbb` library resides. If you have installed the `libtbb` package
of a Linux distribution (like Ubuntu or Debian), make sure to have
installed also the development version (normally called `libtbb_dev`),
otherwise you do not have the header files.  If you have a standard
istallation you may put `/usr/include`, If you use the module system
for the PACS course the value of `mkTbbInc` is set through an
environmental variable.

If you want to compile the parallel version of the example do

    make parallel
    
while simply

    make 
    
produces the scalar version.  Here the paralellelization is trivial,
the code evaluates a polynomial in a large number of points, and those
evaluations are made in parallel in the parallel version. Evaluation
is made with the standard rule (expensive) and the Horner rule (more
efficient). You may note that the advantage of parallelization is
larger in the standard rule, since each evaluation is more costly. But
the Horner implementation is always faster that the standard one, both
in the scalar and in the parallel version. The advantage of Horner
increases as the polynomial degree increases. 

The Makefile has been set so that you always compile with optimization
activated.

**Note** To use the parallel algorithm of the standard library you need to link you code with the Intel treading building block (tbb) library.
This is done automatically by the Makefile. Of course, you must have `libtbb.so` installed in your system! Check!

**Note 2:**  To get significant timings use a high degree polinomial, at least 20.

# What do you learn with this example? #
- That the use of a more elaborate algorithm can give a significant efficiency gain, even for apparently simple problems;
- A use of the new (since C++17) parallel algorithms of the standard library; 
- How to use the `std::transform` algorithm.
- The use of a policy passed as function argument to select the different version of the algorithm for plynomial evaluation.
