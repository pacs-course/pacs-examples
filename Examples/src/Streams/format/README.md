# The std::format utility
The `std::format` utility is a new feature in C++20 that allows you to format strings in a more readable and maintainable way. It is similar to `printf` but with a more modern syntax.

The `std::format` function is defined in the `<format>` header file. It takes a format string and a list of arguments, and returns a formatted string.

## Basic usage
Here is a simple example of using `std::format`:

```cpp
#include <format>
#include <iostream>
  std::string message = std::format("Hello, {}!", "world");
  std::cout << message << std::endl;
```
In the given file you find more significant examples of the `std::format` utility. This utility makes writing formatted output much easier and more readable. Moreover, comparing to C-style formatting, it is type-safe and more secure.

# What do I learn here?
You will learn how to use the `std::format` utility to format strings in C++20. More details may be found in the [C++ reference](https://en.cppreference.com/w/cpp/utility/format/format).
