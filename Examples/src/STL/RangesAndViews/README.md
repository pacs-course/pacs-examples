# C++ Ranges, Views, and Range Adaptors #

## Ranges ##
A **Range** is a concept that refers to anything you can iterate over with a beginning and an end. In C++, ranges provide a more modern and powerful abstraction for dealing with sequences of values. The range library, introduced in C++20, offers several utilities to work with ranges conveniently.
In modern c++ a range is any object that can be used in a range-based for loop. This includes containers, as well as views, and in general any class that implements the `begin()` and `end()` functions, returning valid iterators, or that returns valid iterators when called with `std::begin()` and `std::end()`.
The `end()` function should return what is normally indicated as a `sentinel`, i.e. a value that identify the end of the range. For instance for a container `end()` returns an iterator that is one past the last element of the container. Having generalised the concept of `sentinel` allow to introduce ranges defined by the start and the number of elements, as that returned by `std::ranges::subrange` or `std::ranges::views::take`.

### Key Features: ###
- **Composable:** Ranges can be combined with other ranges or operations to create new ranges.
- **Lazy Evaluation:** Operations on ranges are often lazy, meaning they are evaluated only when needed.
- **Readable and Writeable:** Ranges can be designed for reading, writing, or both.

## Views ##
**Views** are classes that represent special kinds of ranges that do not own the data they operate on. Instead, they provide a view into a range given in the constructor by applying some transformation or filtering.

### Characteristics: ###
- **Non-Owning:** Views reference data owned elsewhere, meaning they are cheap to copy.
- **Lazy Computation:** They compute their elements on-the-fly as you iterate through them.
- **Composable:** Multiple views can be composed to form complex data processing pipelines.

An example of a view is `std::ranges::filter_view`, which takes a range and a predicate and returns a view that only includes elements for which the predicate returns `true`:
    
    ```c++
    #include <ranges>
    #include <vector>
    #include <iostream>
    std::vector<int> nums {1, 2, 3, 4, 5};
    std::ranges::filter_view even_nums{nums, [](int n) { return n % 2 == 0; }};
    // even_nums is a view of nums that only includes even numbers
    for (int n : even_nums) std::cout << n << ' '; // prints 2 4
    ```
     
## Range Adaptors ##
Range **Adaptors** are functions that take one or more ranges as input and produce a new view. They adapt a range by transforming it, such as filtering elements or transforming them.
They can be piped, i.e. chained together, to create complex data processing pipelines. Practically for every view there is a corresponding range adaptor. For instance, the `std::ranges::filter_view` view has a corresponding `std::ranges::views::filter` range adaptor. The previous example may be rewritten as:
    
    ```c++
    #include <ranges>
    #include <vector>
    #include <iostream>
    std::vector<int> nums {1, 2, 3, 4, 5};
    auto even_nums = nums | std::ranges::views::filter([](int n) { return n % 2 == 0; });// pipeline!
    // even_nums is a view of nums that only includes even numbers
    for (int n : even_nums) std::cout << n << ' '; // prints 2 4
    ```

But you can do more complex stuff:
```c++
#include <ranges>
#include <vector>
#include <iostream>
    std::vector<int> nums {1, 2, 3, 4, 5};
    auto even=[](int n) { return n % 2 == 0; };
    auto square=[](int n) { return n * n; };
    auto even_nums_squared = 
       nums | std::views::filter(even) | std::views::transform(square);
    for (int n : even_nums_squared) {
        std::cout << n << ' ';
    }
    // Output: 4 16
```

### Common Range Adaptors:
- `views::filter`: Creates a view that includes only elements for which a predicate returns `true`.
- `views::transform`: Applies a function to each range element and returns a view of the results.
- `views::take`: Produces a view consisting of the first `n` elements of the input range.
- `views::drop`: Creates a view that skips the first `n` elements of the input range.
- `views::reverse`: Produces a view with the elements of the range in reverse order.
- `views::join`: Concatenates a range of ranges into a single view.
- `views::split`: Splits a range into subranges delimited by a separator.

# C++ Constrained Algorithms

C++20 introduced a new suite of algorithms, called constrained algorithms, which are also found in the `std::ranges` namespace. These algorithms are adaptations of the traditional algorithms that work with the concepts and components of the ranges library.

## Features

The main features w.r.t. the traditional algorithms are:

- **Single Range Argument**: Unlike traditional algorithms that require separate iterator arguments to denote the beginning and end of a range, these new algorithms can accept a single range argument.
  ```cpp
  // Traditional algorithm
    std::copy_if(range.begin(), range.end(), out, pred);
    // Constrained algorithm
    std::ranges::copy_if(range, out, pred);
  ```
  
Yet, they can still work with an iterator-sentinel pair if needed, offering flexibility in how ranges are specified.
```cpp
    std::ranges::copy_if(range.begin(),range.end(), out, pred);// still valid!
```

- **Projections**: Projections allow for more sophisticated operations by transforming elements before processing them in the algorithm.
  The are implemented as an additional and optional argument.

  ```cpp
    // Traditional algorithm to sort by absolute values
    std::sort(range.begin(), range.end(), [](int a, int b) {
      return std::abs(a) < std::abs(b);
    });
    // Constrained algorithm with a projection (comparison operator is still required, but here we use the default less-than)
    std::ranges::sort(range, {}, [](int a) { return std::abs(a); });// the projector is the third argument
  ```
Projection can also be adopted to specify the element used in the algorithm. For instance, in the following example, we sort a vector of pairs by the second element of the pair:

```cpp
    std::vector<std::pair<int, int>> v = {{1, 2}, {2, 1}, {3, 3}};
    std::ranges::sort(v, {}, [](auto p) { return p.second; });
```
- **Pointer-to-Member Callables**: These algorithms support pointer-to-member callables, which can specify operations on member variables of class objects within a range.
An example:
```cpp
    struct Person {
      std::string name;
      int age;
    };
    std::vector<Person> people = {{"John", 20}, {"Jane", 30}, {"Bob", 25}};
    std::ranges::sort(people, {}, &Person::age);// the projector is a pointer to member!
    // people is now sorted by age
```


## Benefits

- **Improved Readability**: The code becomes more readable and expressive, as it reduces boilerplate and makes the operations on ranges clearer.

- **Enhanced Composition**: They facilitate better composition of operations (by using projection), which is a key advantage in functional-style programming within C++.
- 
- **Type Safety**: Constrained algorithms enhance type safety by utilizing concepts to enforce compile-time checks on the types they operate on.

## Example

Here is another example of how a constrained algorithm might be used:

```cpp
#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    const std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto even = [] (int i) { return 0 == i % 2; };
    std::vector<int> evens;

    // Using std::ranges::copy_if instead of std::copy_if
    std::ranges::copy_if(numbers, std::back_inserter(evens), even);

    for (int num : evens) {
        std::cout << num << ' ';
    }
    // Output: 2 4 6 8 10
}
```

Finally, with the ranges goodies we have also some concepts. For instance the concept `std::ranges::range` is satisfied by any type that can be quilified as a range.

## A note ##
A view is a view on a range, is not a container whose elements can be changed. This is another thing to keep in mind when using views. For instance, the following code will not compile:
```cpp
#include <ranges>
#include <vector>
#include <iostream>
int main() {
    std::vector<int> nums {1, 2, 3, 4, 5};
    auto even_nums = nums | std::ranges::views::filter([](int n) { return n % 2 == 0; });
    std::ranges::sort(even_nums);// error: no matching function for call to 'sort'
}
```
You have to do as follows:
```cpp
#include <ranges>
#include <vector>
#include <iostream>
int main() {
    std::vector<int> nums {1, 2, 3, 4, 5};
    auto even_nums = nums | std::ranges::views::filter([](int n) { return n % 2 == 0; });
    std::vector<int> even_nums_vec(even_nums.begin(), even_nums.end());// use the view to build a vector
    std::ranges::sort(even_nums_vec);// OK!
}
```
In Utilities/ you have a little utility called `range_to_vector` that does the job for you. You could use it as follows (after having installed it in a suitable directory):
```cpp
#include `range_to_vector.hpp`
...
auto even_nums_vec = range_to_vector(even_nums);// use the view to build a vector
...
```

# What do I learn here ? #
- The use of a very recent addition to the C++ language
- A new way of handling containers and algorithm. Cleaner and more effective. It will probably rapidly become the normal way of programming with the containers of the standard library in modern C++;

