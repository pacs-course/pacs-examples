# Explanation of `overloaded.hpp`

## Overview
The `overloaded.hpp` file defines a utility for implementing the **overloaded pattern** in C++. This pattern is particularly useful when working with `std::variant` and `std::visit`, allowing you to define multiple callable objects (e.g., lambdas) and combine them into a single visitor object.

### Key Components

1. **`overloaded` Struct Template**
   - The `overloaded` struct is a variadic template that inherits from multiple callable objects (e.g., lambdas).
   - It uses the `using Ts::operator()...;` syntax to bring all the `operator()` implementations from the base classes into the derived class.
   - This enables the `overloaded` struct to act as a single callable object that can handle multiple types of arguments.

2. **Deduction Guide**
   - The deduction guide `overloaded(Ts...) -> overloaded<Ts...>;` simplifies the creation of `overloaded` objects by allowing the compiler to deduce the template parameters automatically.
   - This feature is not strictly necessary in C++20 and later, as the compiler can deduce the types without an explicit guide.

### Example Usage
The `overloaded` utility is designed to work seamlessly with `std::variant` and `std::visit`. Below is an example of how to use it:

```cpp
#include <iostream>
#include <iomanip>
#include <string>
#include <variant>
#include <vector>
#include "overloaded.hpp"

int main() {
    using var_t = std::variant<int, long, double, std::string>;
    std::vector<var_t> vec = {10, 15l, 1.5, "hello"};

    // Define the visitor using the overloaded pattern
    auto visitor = apsc::overloaded {
        [](auto arg) { std::cout << arg << ' '; },
        [](double arg) { std::cout << std::fixed << arg << ' '; },
        [](const std::string& arg) { std::cout << std::quoted(arg) << ' '; }
    };

    // Use std::visit to apply the visitor to each element in the vector
    for (auto& v : vec) {
        std::visit(visitor, v);
    }

    return 0;
}
```

### Explanation of the Example
1. **`std::variant` Definition**:
   - `var_t` is a `std::variant` that can hold one of several types: `int`, `long`, `double`, or `std::string`.

2. **Visitor Definition**:
   - The `visitor` is created using the `apsc::overloaded` utility.
   - It combines three lambdas:
     - A generic lambda for all types (`auto arg`).
     - A specialized lambda for `double` values, which formats the output as fixed-point.
     - A specialized lambda for `std::string`, which outputs the string in a quoted format.

3. **Applying the Visitor**:
   - The `std::visit` function is used to apply the `visitor` to each element in the `vec` vector.
   - Depending on the type of the element, the appropriate lambda is invoked.

### Output of the Example
The program will produce the following output:
```
10 15 1.500000 "hello" 
```

### Advantages
- **Type Safety**: The `overloaded` utility ensures that all possible types in the `std::variant` are handled.
- **Readability**: The use of lambdas makes the code concise and expressive.
- **Flexibility**: You can easily extend the visitor by adding more lambdas.

### Requirements
- **C++17 or Later**: The `overloaded` utility relies on features introduced in C++17, such as `std::variant` and fold expressions.
- **C++20 (Optional)**: The deduction guide is not required in C++20 and later, as the compiler can deduce the types automatically.

## Conclusion
The `overloaded.hpp` file provides a powerful and elegant way to implement the overloaded pattern in C++. It simplifies the handling of `std::variant` and `std::visit`, making your code more readable and maintainable.