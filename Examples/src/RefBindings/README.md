# Another example of binding rules #

In this example we show **all  possible combinations** of reference bindings, which we recall here, where with `T` we indicate a generic type,
not a template parameter

- `const T&` binds to both rvalue and lvalues (const and non-const) of type `T`;
- `T&`  binds exclusively and preferably to non-const lvalues;
- `T&&` binds exclusively and preferably to non-const rvalues;
-  a const lvalue can bind only  to const lvalue references (`const T&`);
- `const T&&` binds exclusively and preferably to const rvalues.

Moreover, if a rvalue is constant, a move operation reverts to copy, which makes sense since if something has been declared const it can't 
be moved:

Here b is not moved but copied:

```
const std::vector<int> b{1,2,3,4,5};
std::vector<int> c{std::move(b)};
// b is unchanged
```

*Preferably* means that if it is possible to choose among alternatives, the preferred one is chosen. Let's have

```
void fun(std::vector<int> const &);
void fun(std::vector<int> &&);
```
In the following statement, the parameter of both `fun` functions can bind to the rvalue produced by `move`, but in fact the second is chosen:

```
fun(std::move(a)); // calls fun(std::vector<int> &&)
```

## IMPORTANT NOTE ##

 *This is just an example to show you all the possible combinations. However, normally, one never uses all 4 combinations of function reference parameters!*

You usually have just one or two:

- If you plan to modify the argument, i.e. the parameter is effectively a returned value, you only implement the `T&` version;

- In the other cases:

    - If you need to discriminate between movable objects (non-const rvalues) and the rest you implement the `T&&` and `const T&` versions;
    - If you need a reference (big or polymorphic object) for a quantity not modified by the function and you don't need to discriminate l- and r-values (which is often the case), you implement just the `T const &` version. It will bind to lvalues and rvalues alike;
    - If you do not need to pass by reference (the object is small and fast to copy, and it is not a polymorphic object) you may just implement a function that takes the argument by value: like `double fun(double x)`. It will accept both rvalues and lvalues, in the function scope you have a copy.



## Forwarding (universal) references ##
In this example we report also the magic of forwarding references. They are 
always used together with `std::forward<>` and have the form
```
template <class T>
void fun (T&& y)
{
...
auto x = g(std::forward<T>(y));
```

i.e. the `&&` is *applied to a template parameter*. The strange rules of reference collapsing (which you may also forget) 
and the magic of `std::forward<T>` is such that `g` will get an rvalue if `y` was an rvalue and a lvalue if `y` was an lvalue. That's why it is called "forwarding": it forwards the value category. If move semantic is implemented for `T` (i.e. if `T` has a proper move constructor and move assignment) then `y` will be moved in the parameter of `g` if (and only if) it can be moved safely.

Remember: to have a forwarding (also called universal) reference you
must have the construct indicated above: I repeat the `&&` is applied
to a template parameter. Of course, you may have more than one parameter in your functions, and also more than one forwarding reference.

Often forwarding references are used to avoid overloading a `const T&` and `T&&` version of a function. Without the forwarding reference
to separate the behaviour of `fun` with respect to r- and l-value argument I would need to replicate code:


```C++
template <class T>
void fun (const T& y)
    {
        ...
    auto x = g(y);
    ...
    }

template <class T>
void fun (T&& y)
    {
    ...
    auto x = g(std::move(y));
    ...
    }

```

With a forwarding reference, instead, I have all in one function!.

## A suggestion ##
Try to comment out some of the function definitions in the example and see what happens!

# What do I learn from here #
- The full story about reference bindings;
- How a forwarding reference operates.

