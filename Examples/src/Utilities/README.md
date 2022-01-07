#Various Utilities#

In this directory you have utilities thay may be included and used in
other examples. So, you need to install them before moving to the other
examples.

You need to do

    make 
    make install

to install them. `make` will produce both a dynamic and a static library, called `libpacs.so` and `libpacs.a`, respectively. `make install` installs the header files in `PACS_ROOT/include` and the libraries into `PACS_ROOT/lib`. Use `make DEBUG=no` if you want the library code to be optimised.

After you have installed the libraries you can run

    make test

which produces tests for most of the utilities. All tests start with `test_`. You may have a look at the source to see what they do.

*Remember to install the utilities, they are used by other examples!.*

**Note**: some utilities are in a nested namespace of the namespace `apsc`. Check it out looking at the code or at the examples.

List of the utilities:

* `chrono`  An utility to take times, built on the chrono utilities of the standard library.

* `CloningUtilities` Tools for clonable classes (Prototye design pattern). It contains some type traits to test if a class T containes the (usually virtual) method

```
std::unique_prt<B> clone() const;
```

that returns the pointer to a copy of the object wrapped into a
unique_prt.  B can be either T or a base class of T. It also contains
an interesting class, called `PointerWrapper`, which implements an owning pointer with deep copy
semantic! The "pointed" class should be clonable. It means that you can use it to implement composition of a polymorphic object!

* `cxxversion` To test with which version of C++ you are compiling your code

* `extendedAssert`  Asserts with a message. It extends assert macro so that you can insert a message. There are also swithches that can be activated with the `-DXXX` compiler option to change the behaviour of some of them.

* `Factory`  A generic object factory. Inspired by a code by [Andrei Alexandrescu](https://en.wikipedia.org/wiki/Andrei_Alexandrescu). 

* `GetPot`  GetPot command parser<http://getpot.sourceforge.net/>. I have simplified the version available on the given link, so you have just to so `#include <GetPot>` of `#include "GetPot"`, and have it available.

* `gnuplot-iostream` A stream to open gnuplot from within a program. Useful for simple visualizations within your code. You need[gnuplot](http://www.gnuplot.info/) installed in your system (it is available as debian package).

* `is_complex.hpp` A header file containing a type trait to interrogate is a type is a `std::complex<T>`

* `is_eigen.hpp` A header file containing a type trait to interrogate is a type is a `Eigen::Matrix`


* `JoinVectors.hpp` Just an example on how to imitate the `join` phyton command. You can use it to iterate jointly on a set of vectors. 

* `Proxy.hpp` It is not a proxy (bad naming, sorry). It is an utility that may be used to register objects in an object Factory automatically.

* `readCSV` A class to read csv files. Useful if you have data in a speadsheet and you want to load it into a C++ code. There are better tools than this one around. But this is relativley simple and handy

* `scientific_precision` A function that sets the precision of a stream to the maximum value for a floating point. It contains also stream manipulators for the same purpose.

* `setUtilities` Three utilities to simplify operations on sets (union/difference/intersection)
represented by an ordered container. They are built on top of the analogous
utilities of the Standard Library, but with a simpler interface. 

* `StatisticsComputations` Some tools to compute basic statistics of a sample.

* `string_utility` Some extra utilities for strings: trimming (eliminate useless blanks) and lower-upper conversion. We have recetly added utilities for reading a whole text file in a buffer (it is faster, though potentially memory consuming, and an utility that computed the Levenshtein edit distance between two strings).

* `toString` Converts anything for which there is the `<<` streming operator to a string. A use of `std::stringstream`.


** Note ** `Factory.hpp` and `Proxy.hpp` are in fact links to the same file in the folder `GenericFactory`. If the files are not present for some reason you may safely copy in `Utility/` the files in `GenericFactory/`.


## What you can learn from these examples##

The files in this directory illustrate the advantage of having little general utilities that can be integrated in different codes.
Some utilities are simple to understand, like `Chrono` or `StatisticsComputations`, others make use of more sophisticated generic programming 
techniques, like `Factory`, or template metaprogramming, like `is_complex`, `is_eigen`, `joinVectors` and `CloningUtilities`. 
Finally, some, like `gnuplot_iostream` and `GetPot` are just copies of tools available open source, copied here for simplicity.

In `CloningUtilities` and `joinVectors` you have classes that define a dereferencing operator (`*`) and an access via pointer operator
(`->`). Something you do not find very often.


