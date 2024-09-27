# An example of use of operator overloading. #

We provide a class that stores two integers to represent a rational
number and we have overloaded all arithmetic operators. We also
provide a conversion operator to `double`. So a `Rational` is automatically converted to double.

Automatic conversion to integers is instead suppressed (you can still
do it, but only explicitely). This becouse otherwise you might get
ambigous situations.

Not also that I can initialise a Rational using a `std::ratio<N,M>`
object. `std::ratio` is a tool of the Standard Library to hold
**constant rational numbers**.

# What do I learn here? #
- A rather complete example of overloading of arithmetic operators.

