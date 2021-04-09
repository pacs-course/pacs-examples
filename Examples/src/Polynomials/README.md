#A class that represent polynomials#

Things to note

* The degree of the polynomial should be known at compile time since it is passed by as template parameter. This gives more rigidity, but increases simplicity and efficiency. Of course, you may modify the class to use std::vector instead of arrays and allow dynamic setting of the degree. This may also allow to implement a full algebra. Try to do it!
   
* I want the class to be rather general, I can have polynomial on real or complex fields, and, if the basic arithmetic and conversion operators are set we can also have matrix polinomials.

* I have implemented an output streaming operator for the polynomials, to be able to pretty-print them.
    
* I have implemented an algorithm for polynomial division. Here a few thing should be noted:
	- I have used al lot standard algorithms, even if they can be replaced easily by for loops. This gives more generality, but the main purpose was educational: to show you their use. They can all be made parallel adding an execution policy. Actually, I do not thing it is worthwile, unless you have polynomials of very high degree!
	- The agorithm  works also for complex polynomials;
	- I have used references to access Polynomial members directly, in particular the coefficients;
	- I have used **function template overloading** to treat the case of division by a constant polinomial. Whay it was necessary?
	becouse I have used `unsigned int` as template parameter

* I have implemented a whole set of operators, so that this polynomial class obays the standard rules of polynomial algebra. You can add and multiply polynomials, multiply with a scalar and also elevate the polynomial to an integer power.
	     
* I have also implemented a class template for computing monomials value at compile time.

* In the `main` I have used the `complex literals`. By including `<complex>` and writing

```
  using namespace std::complex_literals;
```
you can write a `complex<double>` as, for instance, `2. + 3.i` (nice isn't it?). A little note however,
`3i` maps into something that is syntactically equivalent to `std::complex<double>{0,3}`, so if you write
`x = 2 + 3i` **you get an error**. This is becouse `2` is an **integer literal** and there is no conversion from `int` to `complex<double>`. You have to write `x=2.0 + 3i`, or `x=2.+3i` or, even better, `x=2.+3.i`. Remember: `2` is an `int`, `2.` is a `double`! If you want to use `long double` for some reason, you need to use `il` instead of `i`:
`x= 2.0L + 3.0il`. For `float` you have `if`.  Again, remember that also literals have a type, and you can specify it with a special suffix.

#What do I learn from this example#
- Some use of integral template parameters;
- Some use of template recursion;
- Another example of operator overloading;
- How the use of automatic return deduction can simplify life in generic programming;
- The complex literals;
- A way to compute the so called long polynomial division (inspired on a code available on [Rosetta Code](https://rosettacode.org/wiki/Rosetta_Code).
   
    
