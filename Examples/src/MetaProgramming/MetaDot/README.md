# An implementatio of the "compile time" dot product of two std::array#

Here, by using template recursion I can let the compiler compute the dot product of two arrays! 

To see the effect you have to compile with the optimization on `make DEBUG=no`. If you use `nm --demangle` you may note that there are no more symbols related to `metaDot<>`: it means that the compiler has made the computation at compile-time. 