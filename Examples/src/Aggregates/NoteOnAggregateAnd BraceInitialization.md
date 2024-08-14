# Aggregates and brace initialization #
## What's an aggregate: an annotated definition ##

An aggregate is a class type (typically a struct), that has 

* no private or protected non-static data members.

It means in practice that all member variables are public, however
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

* no user-provided, inherited or explicit constructors 

It is indeed assumed that if you provide a constructor you want to do something special 
when constructing an object (otherwise, why are you not happy with the synthetic one)? Since c++20
also constructor decalred as default are not admitted (for reasons misterious to me)

* no virtual, private, or protected base classes

However, an aggregate may be derived publicly from another aggregate*

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

### An important point ###
Brace list initialization is avaliable also for vectors and the other dynamic containers 
of the standard library (like sets or lists). But **it is not aggregate initialization** (indeed, those containersa are not aggregates).
See the next section to understand how brace initialization is valid also to std containers that are not aggregates.

### Relation with "Trivially Copy Constructible"

The concept of being "trivially copy constructible" is related but distinct from being an aggregate. A class is trivially copy constructible if:

1. It has a copy constructor that is not user-provided (i.e., the compiler generates it).
2. Its copy constructor is not deleted and is trivial (does not perform any custom operations).
3. All its non-static data members and base classes are themselves trivially copy constructible.

The relationship between aggregates and trivially copy constructible types lies in the simplicity and efficiency of operations. Aggregates, by definition, do not have custom constructors, destructors, or assignment operators, making them candidates for trivial operations, including copy construction. However, being an aggregate does not automatically make a class trivially copy constructible. The class must also meet the criteria for trivial copy construction, which includes having all of its non-static data members and any base classes be trivially copy constructible.

In summary, while all aggregates have the potential to be trivially copy constructible if their members are also trivially copy constructible, not all trivially copy constructible classes are aggregates. The distinction primarily lies in the additional structural restrictions placed on aggregates (e.g., no user-declared constructors) that do not apply to all trivially copy constructible classes.

