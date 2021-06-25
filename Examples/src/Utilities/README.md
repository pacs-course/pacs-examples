#Various Utilities#
you need to do

    make 
    make install

to install them. `make` will produce a dynamic and a static library, called `libpacs.so` and `libpacs.a`, respectively. `make install` installs the header files in `PACS_ROOT/include` and the libraries into `PACS_ROOT/lib`. Use `make DEBUG=no` if you want the library code to be optimised.

After you have installed the libraries you can run

```
make test
```

which produces tests for most of the utilities.  

*Remember to install the utilities, they are used by other examples!.*

**Note**: some utilities are in a nested namespace of the namespace apsc. Check it out looking at the code or at the examples.

List of the utilities:

* `GetPot`  GetPot command parser<http://getpot.sourceforge.net/>. I have simplified the version available on the given link, so you have just to so `#include <GetPot>` of `#include "GetPot"`, and have it available.

* `chrono`  An utility to take times, built on the chrono utilities of the standard library.

* `extendedAssert`  Asserts with a message. It extends assert macro so that you can insert a message. There are also swithches that can be activated with the `-DXXX` compiler option to change the behaviour of some of them.

* `Factory`  A generic object factory. Inspired by a code by [Andrei Alexandrescu](https://en.wikipedia.org/wiki/Andrei_Alexandrescu). 

* `Proxy`    A proxy to add objects to the generic object factory

* `gnuplot-iostream` A stream to open gnuplot from within a program. Useful for visualization. You need `gnuplot` installed in your system.

* `scientific_precision` A function that sets the precision of a stream to the maximum value for a floating point. It contains also stream manipulators for the same purpose.

* `string_utility` Some extra utilities for strings: trimming (eliminate useless blanks) and lower-upper conversion. We have recetly added utilities for reading a whole text file in a buffer (it is faster, though potentially memory consuming, and an utility that computed the Levenshtein edit distance between two strings.

* `readCSV` A class to read csv files. Useful if you have data in a speadsheet and you want to load it into a C++ code.

* `setUtilities` Three utilities to simplify operations on set represented by an ordered container. They are built on top of the analogous
utilities of the Standard Library

* `cxxversion` To test with which version of C++ you are compiling your code

* `CloningUtilities` Tools for clonable classes (Prototye design pattern). It contains some type traits to test if a class T containes the (usually virtual) method

```
std::unique_prt<B> clone() const;
```

that returns the pointer to a copy of the object wrapped into a
unique_prt.  B can be either T or a base class of T. It also contains
an interesting class, called `PointerWrapper`, which implements an owning pointer with deep copy
semantic! The "pointed" class should be clonable. It means that you can use it to implement composition of a polymorphic object!

* `is_complex`  Type traits to check is a type is `std::complex<T>`

* `is_eigen` Type trait to check if a type is an Eigen vector or matrix

* `StatisticsComputations` Some tools to compute basic statistics of a sample.

* `joinVectors.hpp` A poor man implementation of the join() utility in python. It allows to join together standard vectors and iterate on all of them at the same time. Look at `test_joinVector.cpp` to see the possible usage. It is a nice example of variadic templates and iterators. The implementation can be bettered, in particular the design of the iterator. But it works (apparently).


** Note ** `Factory.hpp` and `Proxy.hpp` are in fact links to the same file in the folder `GenericFactory`. If the files are not present for some reason you may safely copy in `Utility/` the files in `GenericFactory/`.

## What you can learn from these examples##
The files in this directory illustrate the advantage of having little general utilities that can be integrated in different codes.
Some utilities are simple to understand, like `Chrono` or `StatisticsComputations`, others make use of more sophisticated generic programming 
techniques, like `Factory`, or template metaprogramming, like `is_complex`, `is_eigen`, `joinVectors` and `CloningUtilities`. 
Finally, some, like `gnuplot_iostream` and `GetPot` are just copies of tools available open source, copied here for simplicity.

In `CloningUtilities` and `joinVectors` you have classes that define a dereferencing operator (`*`) and an access via pointer operator
(`->`). Something you do not find very often.

## IMPORTANT NOTE ##
Some utilities use the new parallel version of the algorithms of the standard library! So you have to make sure that the local Makefile.inc contains the right indication on where the libtbb library can be found. At the moment the local Makefile.inc is

```
# Change to suit your system
mkTbbLib?=/usr/lib/x86_64-linux-gnu/
mkTbbInc?=/usr/include
LIBRARY_NAME=pacs
STATIC_LIBFILE=lib$(LIBRARY_NAME).a
DYNAMIC_LIBFILE=lib$(LIBRARY_NAME).so
LDLIBS+=-L$(PACS_LIB_DIR) -lpacs -L${mkTbbLib} -ltbb
CPPFLAGS+=-I$(mkTbbInc)
.```

If you use the module system `mkTbbLib` and `mkTbbInc` should be set to the right places. If not, make sure that you indicate the place where
the `libtbb.so` and `tbb.h` are set. In an ubuntu system they are in the standard directories `/usr/lib/x86_64-linux-gnu/` and `/usr/include`
