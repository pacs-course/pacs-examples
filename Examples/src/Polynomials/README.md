#A class that represent polynomials.#

Things to note

* The degree of the polynomial should be known at compile time since it is passed by as template parameter. this gives more rigidity, but increases simplicity
   and efficiency. Of course, you may modify the class to use std::vector instead of arrays and allow dynamic setting of the degree. Try to do it!
   
* I want the class to be rather general, I can have polynomial with real or complex coefficients, and, if the basic arithmetic and conversion operators are set
    we can also have polinomials with matrix coefficients. To do so I have implemented the call operator as a method template. 
    A polynomial takes an argument `T` whose type  is determined only when instantiated and I use a type trait to find the common type between the type of the coefficients and 
    of the argument.  `std::commont_type_t<A,B>` returns A if B is convertible to A, B if A is convertible to B. If both are convertible with each other it returns 
    the type where the conversion will not induce loss of precision.
    
* I have also implemented a class template for computing monomials value at compile time.
 
   
    
