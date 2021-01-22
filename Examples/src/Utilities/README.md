#Various Utilities#
you need to do

    make 
    make install

to install them. `make` will produce a dynamic and a static library, called `libpacs.so` and `libpacs.a`, respectively. `make install` installs the header files in `PACS_ROOT/include` and the libraries into `PACS_ROOT/lib`. Use `make DEBUG=no` if you want the library code to be optimised.


`make` also produces tests for most of the utilities.  Remember to install the utilities, they are used by other examples!. 

Note: some utilities are in a nested namespace of the namespace apsc. Check it out looking at the code or at the examples.

* `GetPot`  GetPot command parser. I have now inserted the newest version. You have to include just `GetPot`

* `chrono`  An utility to take times

* `extendedAssert`  Asserts with a message. It extends assert macro

* `Factory`  A generic object factory

* `Proxy`    A proxy to add objects to the factory

* `gnuplot-iostream` A stream to open gnuplot from within a program. Useful for visualization

* `scientific_precision` A function that sets the precision of a stream to the maximum value for a floating point. It contains also stream manipulators for the same purpose.

* `string_utility` Some extra utilities for strings: trimming (eliminate useless blanks) and lower-upper conversion.

* `cxxversion` To test which version of C++ is compiling your code

* `CloningUtilities` Tolls for clonable classes (Prototye design pattern). It contains some type traits to test if a class T containes the (usually virtual) method

```
std::unique_prt<B> clone() const;
```

that returns  the pointer to a copy of the object wrapped into a unique_prt.  B can be either T or a base class of T.

* `is_complex`  Type traits to check is a type is `std::complex<T>`

* `is_eigen` Type trait to check if a type is an Eigen vector or matrix

* `SmartSummation` Some tools that implements way of making sums of vectors reducing roundoff error.

* `StatisticsComputations` Some tools to compute basic statistics of a sample.

* `joinVectors.hpp` A poor man implementation of the join() utility in python. It allows to join together standard vectors and iterate on all of them at the same time.
Look at `test_joinVector.cpp` to see the possible usage. It is a nice example of variadic templates and iterators. At the moment can anly be used to read from the joined vectors. The extensio to allow also writing on them is possible but much more complex. 



** Note ** `Factory.hpp` and `Proxy.hpp` are in fact links to the same file in the folder `GeneriFactory`. If the files are not present for some reason you may safely copy in `Utility/` the files in `GenericFactory/`.