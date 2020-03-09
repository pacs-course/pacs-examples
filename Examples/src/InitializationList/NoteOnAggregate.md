# What's an aggregate: an annotated definition #

An aggregate is a class type (typically a struct), that has 

* no private or protected direct (since C++17) non-static data members.
    
*it means in practice that all member variables are public, however
you may still have private or protected member variables but only if
they are static:*

    struct s
    {
    int a;
    int b;
    protected:
    static const double c=3.5;
    };


*is still an aggregate*

* no user-provided, inherited or explicit constructors (explicitly defaulted or deleted constructors are allowed) 

*It is indeed assumed that if you provide a constructor you want to do something special when constructing an object (otherwise, why are you not happy with the synthetic one)?* 

* no virtual, private, or protected base classes

*Since C++17 an aggregate may derive publicly from another aggregate*

* no virtual member functions 

**Note** The defintion has changed in the various version of the compiler. A full explanation is found on any good reference of C++
