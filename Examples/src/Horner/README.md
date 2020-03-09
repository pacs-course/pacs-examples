# Example on timing. We show the advantage of Horner rule.#

We use the utility chrono and the library libpacs.

You should firse enter the directory Examples/src/Utilities and then run

``
make library
make install
``

Make sure that ``Makefile.inc`` in the PACS root directory is set correctly.

## New: Parallel version##
This new version exploits also a parallel version of an algorithm
(``std::transform``) of the standard library. However the
implementation of concurrency is not yet fully supported by all
compilers and architecture. If you use gnu compilers, you must have
`g++` at least version 9. And, on intel or intel-compatible
architecture you need a recent version of the `tbb` libraries.

To have the system correctly set for the parallel version, you need to set up correctly
some variable in the **local** `Makefile.inc` file (or create tcorresponding environment variables)

* `mkTbbLib` should contain the directory where `libtbb.so`
resides. If you have a standard istallation you may put `/usr/lib`, If
you use the module system for the PACS course this value is set
through an envoronmental variable.

* `mkTbbLib` should contain the directory where the header files of
the `tbb` library resides. If you have installed the `libtbb` package
of a Linux distribution (like Ubuntu or Debian), make sure to have
installed also the development version (normally called `libtbb_dev`),
otherwise you do not have the header files.  If you have a standard
istallation you may put `/usr/include`, If you use the module system
for the PACS course the value of `mkTbbInc` is set through an
envoronmental variable.

If you want to compile the parallel version of the example d

    make parallel
    
while simple

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




**Note 1:** Since ``std::pow(x,n)`` may be very expensive (since it
treats also non-enteger exponents), I have implemented in
``horner.cpp`` a function for elevation to power with integer
exponents that just perfoms the required multiplications. This makes
the comparison between standard and Horner algorithms fairer (Horner's
algorithm does not need ``pow`` ). However, since c++17 there is an
overloading of ``pow`` for integer exponents, so maybe there is no
need of the specialised user-defined version anymore (but so far the
implementations of the standard library do not exploit the
specialization).

**Note 2:**  To get significant timings use a high degree polinomial, at least 20.
