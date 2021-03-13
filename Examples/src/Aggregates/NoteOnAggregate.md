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
    inline static const double c=3.5;
    };


*is still an aggregate*

* no user-provided, inherited or explicit constructors (explicitly defaulted or deleted constructors are allowed) 

*It is indeed assumed that if you provide a constructor you want to do something special when constructing an object (otherwise, why are you not happy with the synthetic one)?* 

* no virtual, private, or protected base classes

*Since C++17 an aggregate may be derived publicly from another aggregate*

* no virtual member functions 


**Note** The defintion has changed in the various version of C++. A full explanation is found on any good reference of C++

## What can you do with an aggregate? ##

- You can use list (brace) initialization, which in this contest is called *aggregate initialization*.

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

#An important point#
Brace list initialization is avaliable also for vectors and the other dynamic containers of the standard library (like sets or lists). But **they are not aggregates**.
In their case the brace initialization is possible because they have a special constructor that accepts a so called `initializer_list`, look [here](https://en.cppreference.com/w/cpp/utility/initializer_list) if you want details on initialiser lists. In a aggregate you do not have to specify any constructor and you have the brace initialization automatically (actually, if you have a constructor... the class in not an aggregate anymore!).



