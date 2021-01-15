# What's an aggregate: an annotated definition #

An aggregate is a class type (typically a struct), that has 

* no private or protected direct (since C++17) non-static data members.
    
* it means in practice that all member variables are public, however
you may still have private or protected member variables but only if
they are static: *the following struct*:

    struct Aggr
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

*Since C++17 an aggregate may be derived publicly from another aggregate*

* no virtual member functions 

**Note** The defintion has changed in the various version of the compiler. A full explanation is found on any good reference of C++

## What can you do with an aggregate? ##

- You can use list (brace) initialization

```
Agg a{3,4}; // Now Aggr.a=3 and Aggr.b=4
// or also
Aggr b = {4,5};
```
- You may also use the argument list when returning an aggregate

```
Aggr fun(double const& x...)
{
int k,l;
...
return {k,l};
}
...

```
- You can use structured bindings to get the values from an aggregate

```
auto [i,j] = fun(...)//  i is equal to the value of member a of the Aggr object returnd by fun
```
- you can interface with C programs easily: an aggregate has standard memory layout

## What you cannot do with an aggegate ##

All the more complex operations allowed for a full fledged class object.


