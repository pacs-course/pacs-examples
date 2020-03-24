#A simple example of the Curiously Recursive Template Pattern (CRTP)#

CRTP is based on the fact the the construct

``class Derived1: public Base<Derived1>``

where derived class is the template argument of the base class, is valid in C++.

This allows to implement what is often calls "static polymorphism". A
form of polymorphism much less flexible than the standard one
(everything must be resolved at compile time), but particularly
efficient if methods of the class are used many times in the
program. Indeed, with this technique we avoid the call to a virtual
method, which introduces some overhead. 

Being a "compile-time" (static) technique, however, it does not have
the same flexibility as traditional style polymorphism via virtual
methods.


It is used by the EIGEN library extensively. Indeed, it is the base for
an efficient implementation of Expression Templates.





