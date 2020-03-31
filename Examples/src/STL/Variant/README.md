#The std::variant<Parms...> utility#

the `std::variant<...>` utility is a recente extension of C++ that provides a safer and simple `union`.  It defiens a variable that may stare different types.

In the example we show some basic usage and also the tool `std::visit`, normally use together with `std::variant`, to visit a container of `std::variant` and perform different operations according to the type actually stores in each element. 