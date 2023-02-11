# A simple example of the Curiously Recursive Template Pattern (CRTP) #

CRTP is based on the fact the construct

``class Derived1: public Base<Derived1>``

where the derived class is the template argument of the base class, 
is perfectly valid in C++.

This allows to implement what is often called "static polymorphism". It is a
form of polymorphism particularly
efficient if methods of the class are used many times in the
program. Indeed, with this technique we avoid the call to a virtual
method, which introduces some overhead. 

Being a "compile-time" (static) technique, however, it does not have
the same flexibility as traditional style polymorphism via virtual
methods.


It is used by the **Eigen** library extensively, 
and in many other high performance libraries. Indeed, it is the base for
the efficient implementation of *Expression Templates*.

In this example I show some possible usage

- In `inequality.hpp` I have a templated version of a decorator that expands
the interface of a class where I have implemented `operator<()` so that it automatically gets all 
other 5 relational operators in a consistent way. Nice if I have several classes to which I 
want to define the full relational operators by defining just `<`.
- In `matrixProxy.hpp` I define another decorator, `crpt_matrix` that increases the interface of 
a class for matrices where I have defined `operator()(int,int)` so that I can access the
same element as `m[i][j]`. It is a nice trick. Look at the code carefully.


## What do I learn from this example? ##

- An interesting (and rather surprising) template metaprogramming construct


