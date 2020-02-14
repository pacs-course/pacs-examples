#A simple example of the Curiously Recursive Template Pattern (CRTP)#

CRTP is based on the fact the the construct

``class Derived1: public Base<Derived1>``

where derived class is the template argument of the base class, is valid in C++.

This allows to implement what is often calls "static polymorphism". A
form of polymorphism much less flexible than the standard one
(everything must be resolved at compile time), but particularly
efficient if methods of the class are used many times in the
program. Indeed, the call to a virtual method introduces some
overhead.


It is used extensively by the EIGEN library. Indeed it is the base for
an efficient implementation of Expression Templates





