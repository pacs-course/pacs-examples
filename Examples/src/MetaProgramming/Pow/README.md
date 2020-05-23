# Metaprogramming techniques for a better Pow#

This example shows how metaprogramming can improve the efficiency of
your code.  

You need first to compile and install the Utilities (in the named directory), since in some examples I use the Chrono utility to compute elapsed time.

To have the real full performance you need to compile with optimization on

make DEBUG=on

Compare the results with and without debugging. You may note the high cost of `std::pow`. The reason is that `std::pow` computes the power for any valid exponent, which in general can be a floating point type. This requires resolving a fixed point iteration! The power functions you find here work only for integer exponents, and convert the elevation to an integer power into a succession of multiplications. And this, if the exponent is not too large, is much more efficient. 

*Note* Things may have changed with c++17 since now the standard accounts for a specialization of `std::pow` with integer arguments, so you may not need this technique anymore (but it very much depend on the actual implementation of the standard library in your compiler, at the moment the performance of std::pow with integer exponent is still poor compared to the recursive version I have implemented here).

I have now added also a function `apsc::pow` in `generalPow.hpp` that, by using type-traits select automatically the best choiche.

**Note**: However, things are changing, the implementations of std::pow is much better in the newer versions of the standard library. They detect if the exponent is an integer (like I do in my general version) and they redirict the algorithms to different implementations. So, `std::pow` is not necessarily a bottleneck. Of corse, you have to activate optimization if you want to gain. 



