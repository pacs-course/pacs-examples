# Another example of binding rules #

In this example we show **all  possible combinations** of reference binding, which we recall here, where with `T` we indicate a generic type,
not a template parameter

- `const T&` binds to both rvalue and lvalues (const and non-const) of type `T`
- `T&`  binds exclusively and preferably to non-const lvalues
- `T&&` binds exclusively and preferably to rvalues
- a const lvalue can bind only  to const lvalue references (`const T&`)
- `const T&&` binds exclusively and preferable to const rvalues

Moreover, if a rvalue is constant, a move operation revert to copy, which makes sense since if something has been declared const it can't 
be moved:

Here b is not moved but copied:
```
const std::vector<int> b{1,2,3,4,5};
std::vector<int> c{std::move(b)};
// b is unchanged
```

'Preferably' means that if it is possible to choose among alternatives, the preferred one is chosen.

```
void fun(std::vector<int> const &);
void fun(std::vector<int> &&);
...

fun(std::move(a));
```
Here the parameter of both `fun` can bind the rvalue produced by `move`, but in fact the second is chosen.

**NOTE:** *This is just an example to show you all the possible combinations. However, normally, one never uses all 4 combinations of function reference parameters!*
You usually have just one or two:

    - If you plan to modify the argument, i.e. the parameter is effectively a returned value, you only implement the `T&` version;
In the other cases:

    - If you need to discriminate between rvalues and lvalues you implement  the `const T&` and `T&&` versions;
    - If you need a reference (big or polymorphic object) but you don't need to discriminate (which is often the case), you implement just the `T const &` version. It will bind to rvalues and rvalues;
    - If you do not need to pass by reference (the object is small and fast to copy, and it is not a polymorphic object) you may just implement a function that takes the argument by value: like `double fun(double x)`. It will accept both rvalues and lvalues, in the function scope you have a copy.



## Forwarding (universal) references ##
In this example we report also the magic of forwarding reference. They are 
always used together with `std::forward<>` and have the form
```
template <class T>
void fun (T&& y)
{
...
auto x = g(std::torward<T>(y));
```

i.e. the `&&` is *applied to a template parameter*. The strange rules of reference collapsing (which you may also forget) 
and the magic of `std::forward` is such that `g` will get an rvalue if `y` was an rvalue and a lvalue if `y` was an lvalue. That's why it is called "forwarding": it forwards the value category.

Remember: to have a forwarding (also called universal) reference you
must have the construct indicated above: I repeat the `&&` is applied
to a template parameter. Of course, you may have more than one parameter.




