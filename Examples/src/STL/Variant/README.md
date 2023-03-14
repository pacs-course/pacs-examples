# The std::variant<Parms...> utility #

the `std::variant<...>` utility is a recent 
extension of C++ that provides a safer and simple `union`.  It defines a variable that may stare different types.

It allows for instance, to have a function that returns different types:
```c++
std::variant<double,std::complex<double>> fun();
...

auto v = fun();
if (std::holds_alternative<double>(v))
    {
    // the variant holds a double. Get it
    double x = std::get<double>(v);
    ...
```
An interesting use of variants is in conjuction with the utility `std::visit`, which allow to implement a visitor design pattern.
In `simpleVisitor.hpp` you have a basic usage of `std::visit`. While, in `manyVariants.hpp` you exploit the fact that `std::visit` may take more than one variant argument!.

`std::visit` and `std::variant` allow to implement a sort of **runtime polymorphysm**. Have a look at  [this page](https://www.cppstories.com/2020/04/variant-virtual-polymorphism.html/).

In the `main` I show also an advanced used of variadic template that allow to create visitors on the fly. This special template  (called `overloaded`) is also contained in the header file `overloaded.hpp` of the `Utilities/` folder.



# What do I learn here? #

- An interesting utility of modern C++.
- A way to implement a runtime polymorphism via  `variant` and `visit`
- A use of variadic templates 	