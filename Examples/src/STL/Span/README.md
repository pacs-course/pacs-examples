# The std::span utility #
`std::span` is a utility introduced in C++20 that provides a view over a contiguous sequence of elements. It is a lightweight, non-owning reference to an array or a portion of an array, which can be used to pass arrays or parts of arrays to functions without copying the data.

Its main purpose is to provide a safe and efficient way to work with arrays and array-like containers, such as `std::array`, `std::vector`, and built-in arrays, without the need to copy or manage memory.

## Key Features ##
- Non-owning: `std::span` does not own the elements it references, meaning it does not manage the memory of the underlying data.
- Contiguous: The elements referenced by `std::span` are guaranteed to be contiguous in memory.
- Bounds-safe: Provides bounds-checked access to elements when using the at method.
- Flexible: Can be constructed from arrays, `std::array`, `std::vector`, and other contiguous containers.

## Usage ##
Here are some common use cases and examples of how to use `std::span`:

Creating a std::span
You can create a std::span from various types of containers:
```cpp
#include <span>
#include <vector>
#include <array>
#include <iostream>

int main() {
    // From a C-style array
    int arr[] = {1, 2, 3, 4, 5};
    std::span<int> span1(arr);

    // From a std::array
    std::array<int, 5> stdArr = {1, 2, 3, 4, 5};
    std::span<int> span2(stdArr);

    // From a std::vector
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::span<int> span3(vec);

    // Output elements
    for (int i : span1) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}
```
Slicing a std::span
You can create sub-spans (slices) from an existing std::span:
```cpp
#include <span>
#include <iostream>

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    std::span<int> span(arr);

    // Create a sub-span from index 1 to 3
    std::span<int> subSpan = span.subspan(1, 3);

    // Output elements of the sub-span
    for (int i : subSpan) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}
```


Bounds-checked Access
You can access elements with bounds checking using the at method:
```cpp
#include <span>
#include <iostream>

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    std::span<int> span(arr);

    try {
        std::cout << span.at(2) << std::endl; // Outputs: 3
        std::cout << span.at(5) << std::endl; // Throws std::out_of_range
    } catch (const std::out_of_range& e) {
        std::cerr << "Out of range error: " << e.what() << std::endl;
    }

    return 0;
}
```

## Conclusion ##
`std::span` is a versatile and efficient utility for working with contiguous sequences of elements in C++. It provides a safe and convenient way to pass arrays and array-like containers to functions without copying data, making it a valuable addition to the C++ standard library.
## What do I learn Here? ##
A nice utility that can be helpsul when is necessary to mix C-style arrays and C++ contiguos containers.