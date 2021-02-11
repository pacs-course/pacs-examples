#An example of use of reference wrappers.#

Firstly, we show the use of `std::ref()` to be able to pass a reference to a function instantiated from a template of the form

    template <class T>
    double fun(T x);

If you want to resolve `T` as a reference, and thus be able pass a
value by reference, you may use `std::ref()` (or `std::cref()` for
const references).  There is no other way, unless you write something
equivalent to `std::ref`. But why bother?

Secondly, we show the use of `reference_wrapper<T>` to store objects
that behaves as reference in a `std::vector<T>` (I recall that you
cannot store references in a `vector<T>` directly).

**Note** The magic of `std::ref()` applies only to template function
parameters, as in the description above. If you have a non template
function that takes the argument by value, like

    double fun(double x)

there is no way to pass the argument as a reference.

#What do I learn here#
- How to pass by reference to a tempalte function that takes the template argument by value (but why have not you used
`double fun(const T& x)` ?
- The magic of reference wrappers, if you want a container of references!.
 
