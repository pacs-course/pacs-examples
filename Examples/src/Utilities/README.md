#Various Utilities#
you need to do

    make
    make install

to install them. You will also produce tests for most of the utilities

* `GetPot`  GetPot command parser. I have now inserted the newest version. You have to include just `GetPot`, the other files (`GetPot.hpp` and `GetPot_imp.hpp`) are helper files that are *included* in `GetPot`.

* `chrono`  An utility to take times

* `extendedAssert`  Asserts with a message. It extends assert macro

* `Factory`  A generic object factory

* `Proxy`    A proxy to add objects to the factory

* `gnuplot-iostream` A stream to open gnuplot from within a program. Useful for visualization

* `scientific_precision` A function that sets the precision of a stream to the maximum value for doubles

* `string_utility` Some extra utilities for strings: trimming (eliminate useless blanks) and lower-upper conversion.

* `cxxversion` To test which version of C++ is compiling your code

