# The mdspan view

`std::mdspan` is a C++23 utility that provides a multidimensional view over a contiguous sequence of elements. It is a lightweight, *non-owning* reference to data that can be interpreted as a multidimensional array, similar to how `std::span` works for 1D arrays.

## Key Features

- **Non-owning**: `std::mdspan` does not own the underlying data; it only provides a view over it.
- **Multidimensional**: Supports arrays of arbitrary dimensions (1D, 2D, 3D, etc.).
- **Flexible layouts**: Supports both row-major (`std::layout_right`) and column-major (`std::layout_left`) memory layouts.
- **Compile-time and runtime extents**: Dimensions can be fixed at compile time or determined at runtime.
- **Efficient**: Zero-overhead abstraction with no dynamic memory allocation.
- **Safe indexing**: Provides multidimensional indexing via the `[i, j, k, ...]` syntax.

## Usage

You can create an `mdspan` from various data sources:

```cpp
#include <mdspan>
#include <vector>

std::vector<double> data(12);
std::iota(data.begin(), data.end(), 1.0);

// Create a 3x4 matrix view (row-major, the default)
auto matrix = std::mdspan(data.data(), std::extents<std::size_t, 3, 4>{});

// Access elements
for(std::size_t i = 0; i < matrix.extent(0); ++i) {
    for(std::size_t j = 0; j < matrix.extent(1); ++j) {
        std::cout << matrix[i, j] << " ";
    }
}
```

## Content of main_mdspan.cpp

The `main_mdspan.cpp` file demonstrates several use cases of `std::mdspan`:

1. **Example 1**: Basic 2D matrix with fixed compile-time extents (3×4)
2. **Example 2**: Dynamic extents determined at runtime (4×5)
3. **Example 3**: Mixed fixed and dynamic dimensions (3 rows fixed, 5 columns dynamic)
4. **Example 4**: Column-major layout (using `std::layout_left`) with fixed columns and dynamic rows
5. **Example 5**: Vector operations using 1D mdspan views
6. **Example 6**: Dot product computation using 1D vector views

Each example illustrates how to:
- Create `mdspan` objects with different extent configurations
- Fill data using `std::iota`
- Access and manipulate elements using multidimensional indexing
- Switch between row-major and column-major memory layouts

## AN IMPORTANT NOTE
At far as February 2026 not all compilers support std::mdspan/. A compliant compiler is clang++ version 18 when *linked with the corresponding libc++ imolementation of the standard library". To compile this code on Ubuntu 24.04 I had to modify the compiler to use `clang++` with `-stdlib=libc++`. The required modifications are in the local `Makefile.inc`

## What You Learn

- How to use multidimensional array views 
- The difference between fixed and dynamic extents
- How `mdspan` provides a safe, efficient alternative to manual pointer arithmetic
- Integration with standard library containers like `std::vector`