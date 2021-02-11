# Constexpr #

A simple example of a constexpr function and the difference with a
normal function.

Run `make` and then `examine.sh` to see that the normal function `cube`
creates a symbol, so the corresponding machine code that calls the
function has a jump-and-return to the function address, while
cubeconst is always resolved inline. Indeed it does not appear when
running nm.

Then the two versions that uses a constexpr function are compared in
size. You may note that the one that simply inlines the function is
larger. The one that resolve the value statically is smaller since the
value returned by `cubeconst` is computed directly at compile-time.

Finally, in `HornerConstexpr.hpp` you have an implementation of the
Horner algorithm for computing polynomials that thanks to template
programming, use of automatic type deduction and constexpr is able to
operate with real and complex floating points, and resolves the
computation statically if the arguments are constant expressions.

#What do I learn here?#

Some use of constexpr functions. Constexpr functions are implicitely inlined (thus they are defined in a header file) and have the property
that the returned value can be computed directly at compile time if the arguments are constant expressions. There are limitations, well described on any C++ reference. And, remember, if the function is too complicated the compiler gives up trying to inline it, and it produces an ordinary function.

*It makes sense to make constexpr short functions that you expect to be called a lot of time in your code. Otherwise, the gain is neglibible!*
