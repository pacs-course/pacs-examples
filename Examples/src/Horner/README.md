# Example on timing. We show the advantage of Horner rule.#

We use the utility chrono and the library libpacs.

You should firse enter the directory Examples/src/Utilities and then run

``
make library
make install
``

Make sure that ``Makefile.inc`` in the root directory is set correctly.

Then type
``make DEBUG=no`` 
if you want optimized program
or
``make DEBUG=yes`` or simply ``make`` if you want non-optimized program

*Remember: make clean before you recompile with different optimization*

Alternatively you may type

``make OPTFLAGS='-O3 -ffast-math'``

to compile with maximal optimization.

To tell the loader that you need to look in the library directory of
the course, type

``export LD_LIBRARY_PATH+=../../lib``

before launching the main program. *It should not be necessary now
because I have changed the Makefiles to use the ``-Wl,-rpath`` option.*

**Note 1:** Since ``std::pow(x,n)`` may be very expensive (since it
treats also non-enteger exponents), I have implemented in
``horner.cpp`` a function for elevation to power with integer
exponents that just perfoms the required multiplications. This makes
the comparison between standard and Horner algorithms fairer (Horner's
algorithm does not need ``pow`` ). However, since c++17 there is an
overloading of ``pow`` for integer exponents, so maybe there is no
need of the specialised user-defined version anymore.

**Note 2:** I have also implemented a parallel version that exploits
parallel ``std::tansform`` algorithm. But few compiler support parallelization
yet, so it has been commented out. You may uncomment it and change the
test to see how much you gain with parallelization, if you have a very
recent compiler (g++ version 9 should work, but read the instructions
on the web since other components of your OS may need upgrading.)
