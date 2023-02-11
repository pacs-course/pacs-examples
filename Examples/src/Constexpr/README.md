# Constexpr #

A simple example of a constexpr function and the difference with a
normal function.

Run `make` and then `examine.sh` to see that the normal function `cube`
creates a symbol, so the corresponding machine code that calls the
function has a jump-and-return to the function address, while
`cubeconst` is always resolved inline when called with a constant expression. 
Indeed, it does not appear when running  `nm` on `mainConstexprFunction`.

Then the two versions that uses a constexpr function are compared in
size. You may note that when compiled without optimization 
mainConstexprFunction2 is larger while 
mainConstexprFunction is smaller since the
value returned by `cubeconst` is resolved directly at compile-time.

However, if you use aggressive optimization, like -O3, the compiler is able 
(in this simple case) to treat the version with const double (`mainConstexprFunction2`)
as the one with constexpr double (`mainConstexprFunction`). 
The machine code has exactly the same size, so it is very likely identical.

Finally, in `HornerConstexpr.hpp` you have an implementation of the
Horner algorithm for computing polynomials that, thanks to template
programming, the use of automatic type deduction and constexpr, is able to
operate with real and complex floating points, and may resolve the
computation statically if the arguments are constant expressions, and is
inlined in the other cases.

If you compile `mainHornerConstexpr` with

``````
make -B mainHornerConstexpr DEBUG=yes
``````
you may note that `nm --demangle mainHornerContexpr | grep Horner` returns 
the instances of `HornerConstexpr` generated with `std::vector`. Indeed,
an object of type `std::vector` cannot be a constant expression (it's allocated dynamically!)
so the compiler has generated the function. While, all instances where `HornerConstexpr`
was instantaed with a `std::array` do not appear as symbols! It means that the compiler has
reolved the expression at compile time! No function is called at run time.

Diffrerently, if I use optimization
```
 make -B mainHornerConstexpr DEBUG=no
```
`nm` returns (at least on my computer) no symbols named `HornerConstexpr`. It means that 
the instances with vectors have been effectively inlined. The corresponding code *is computed at run time*
but integrated in the calling code, not as a separate function.

# What do I learn here? #

Some use of constexpr functions. Constexpr functions are implicitly inlined (thus they are defined in a header file) and have the property
that the returned value can be computed directly at compile time if the arguments are constant expressions. 
There are limitations, well described on any C++ reference. And, remember, if the function is too complicated the compiler gives up trying to inline it, and it produces an ordinary function.

*It makes sense to make constexpr short functions that you expect to be called a lot of time in your code. Otherwise, the gain is neglibible!*
