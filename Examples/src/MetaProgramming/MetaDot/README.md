# An implementation of the "compile time" dot product of two std::array #

Two versions of an utility that computes the dot product o two arrays, at compile time (if possible, otherwise via inlining)

The second version is more general since it treats also arrays of complex numbers. We have used the `is_complex<T>` trait defined in 
`Utility/is_complex.hpp`,

To see the effect you have to compile with the optimization on `make DEBUG=no`. If you use `nm --demangle` you may note that there are no more symbols related to `metaDot<>`: it means that the compiler has made the computation at compile-time. 

# What do you learn here? #
- Another example of template recursion
- An example of constexpr function with the use of `if contexpr` to select the branch at compile-time
- Use of concepts to select the correct branch

**A technical note** 
The `conj` operator for a complex number has been made constexpr only since C++20. So, if your compiler is not c++20 compiant `metadot17` will be only inlined but not resolved compile time when used with arrays of complex numbers.