#The Visitor Design Pattern#

The Visitor pattern implements double-dispatching: the actual method
called depend both on the type of the caller and that of the class
providing the method.

C++ has no direct mechanism for double-dispatching. But has two
important mechanisms: 

    * Function/methods overloading By which a different collable object is chosen on the basis arguments type;
    * Polymorphism. By which a different virtual method is chosen depending on the actual type of a polymorphic object.
    
This example shows how by combining those two concept we may implement
double dispatching.
