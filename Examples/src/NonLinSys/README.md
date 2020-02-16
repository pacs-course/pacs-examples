#An example of use of callable objects#

In `NonLinearSystem.hpp` you find a class that models a system of non linear equations. It is basically composed by an array of callable objects and it provides an interface to call them
at a given point and computing some useful quantities.

In `jacob.hpp` you find an abstract class that models a Jacobian. The Jacobian is return as an Eigen matrix. A derived class has been implemented that given an existing `NonLinearSystem` object, 
computes the Jacobian via finite differences.

In Newton.hpp a Newton method has been implemented as a function.

In file `fixepoint.hpp` the implementation of a function that solves F(x)=0 by a simple fixed point iteration.

This example can be bettered greatly. First of all one can implement a generic fixed point iterator (Newton is a particular fixed point iteration!) capable of
taking as a policy any function PH(x) so that PH(x)=x is equivalent to F(x)=0. To be done.
