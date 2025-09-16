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

## The spaceship operator ##
We also provide the spaceship operator `<=>` that is a new feature of
C++20. This operator returns an integer that is negative if the left
operand is smaller, positive if the right operand is smaller, and zero
if they are equal. 

More importantly, by defining the spaceship operator we automatically
get the `==`, `!=`, `<`, `>`, `<=`, `>=` operators.

For completeness, I report in the code also what should be done to implement consistent comparison operators if you do not want (or cannot) use the spaceship operator.

# What do I learn here? #
- The design of a fully working class that represents a rational number;
- The overloading of arithmetic operators;
- A rather complete example of overloading of arithmetic operators and the use of the spaceship
 operator;

