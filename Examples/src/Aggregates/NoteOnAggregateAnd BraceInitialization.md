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

# Brace (uniform) initialization #

With brace initialization we indicate the effort tha C++ has made since c++11 to provide a uniform way to initialize objects. This has very important implications that makes C++ programming simpler and safer, particularly in generic programming. The common characteristics is the use of braces `{}` instead of the 
parentesis `()`. However the meaning of the braces may be different

- POD data
``````
double a{10.}
``````
is just equivalent to `a(10.)`. The only difference is that braces do not allow narrowing, i.e. implicit conversions with potential loss of precision (which is a good safety measure):

``````
double c=5.; // this is just a copy-initialization, perfectly valid
float a{c}; // ettor double -> float conversion loses precision 
``````

- Initialization via constructors
`std::complex<T>` has a constructor that takes to values of type `T`. Thus,

``````
std::complex<double> c{3.,4.};
``````
is equivalent to 

``````
std::complex<double> c(3.,4.);
``````
but. again, narrowing is not allowed.

- Aggregate initialization. We have just discussed it. Valid for aggregates and, in particular, C-style arrays and `std::array`.
For `std::array` there is a little caveat, since a `std::array` is an aggregate containing another aggregate (a C-style array). So sometimes you need to double the braces. I leave out those details here. In doubt, consult a good reference!

- Initializer list initialization.
`std::initializer_list<T>` is a template aggregate defined by the standard library with the purpose to extend to container-type objects
the brace initialization syntax already available for fixed sixe C-style arrays and `std::array` (which are both aggregates). Indeed, if you look in a reference manual, you see that, for instance, `std::vector` has a constructor with signature
``````
vector(std::initializer_list<T> init,const Allocator& alloc = Allocator());
``````
Let's forget about the allocator, which has a default value. Here `init` is a `initializer_list` and is the tool that allows 
``````
std::vector<double> a{1.,2.,3.};
// as well as
std::vector<double> b={-1.,-2.};
``````

Brace initialization has several advantages and *should be preferred to the old parenthesis*. A part a few cases, notably `vector<int> a(2,10))` (a vector of 10 elements equal to 2)  cannot be replaced by `vector<double> a{2,10}` (a vector of 2 elements equal to 2 and 10). 

