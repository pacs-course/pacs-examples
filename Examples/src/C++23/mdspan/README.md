# C++23 `std::mdspan` example

This program demonstrates `std::mdspan`, a non-owning multidimensional view
over contiguous data. The elements live in `std::vector`; each `mdspan` only
describes how that storage should be indexed and does not copy or own it.

## Build and run

Build directly with Clang 20 and libc++ (which provides `<mdspan>` on this machine):

```sh
clang++-20 -std=c++23 -stdlib=libc++ -Wall -Wextra -pedantic main_mdspan.cpp -o main_mdspan
./main_mdspan
```

Remove the executable with:

```sh
make clean
```

## What the code demonstrates

- A 2-by-4 view using the default `std::layout_right` layout, which stores
  elements in row-major order.
- A 4-by-2 view over the same eight integers using `std::layout_left`, which
  stores elements in column-major order.
- Static extents with `std::extents<std::size_t, 2, 4>`.
- Runtime extents with `std::dynamic_extent`.
- A three-dimensional 2-by-3-by-4 view.

## How `std::mdspan` works

An `mdspan` combines a pointer to existing storage with an extents object and
a layout policy. Its rank is the number of dimensions, while each extent is
the size of one dimension. Indexing such as `view[i, j]` translates the
multidimensional indices into an offset in the underlying contiguous storage.

Because `mdspan` does not own its elements, the referenced vector must remain
alive and large enough for every access through the view.
Operations that reallocate the vector invalidate the view's pointer. Indexing
does not provide bounds checking, so every index must be within its extent.

## Template parameters

The template has four parameters; the last two have defaults:

```cpp
std::mdspan<ElementType, Extents,
            LayoutPolicy = std::layout_right,
            AccessorPolicy = std::default_accessor<ElementType>>
```

| Parameter | Options and meaning |
| --- | --- |
| `ElementType` | The element type, such as `int`. Use `const int` for read-only access to the elements. |
| `Extents` | Specifies the index type, rank, and dimension sizes. `std::extents<std::size_t, 2, 4>` fixes both sizes at compile time; `std::dextents<std::size_t, 2>` gives two runtime sizes. Mix them with `std::extents<std::size_t, 2, std::dynamic_extent>`. |
| `LayoutPolicy` | Maps indices to storage offsets. `std::layout_right` (default) is row-major: the last index varies fastest. `std::layout_left` is column-major: the first index varies fastest. `std::layout_stride` supports explicit strides, for example for padded rows. |
| `AccessorPolicy` | Controls how a storage handle and offset access an element. The default uses an ordinary pointer and returns an element reference; custom accessors can provide specialized access behavior. |

For example, given a pointer `data` to enough elements:

```cpp
// Static shape, column-major layout: no runtime sizes are needed.
std::mdspan<int, std::extents<std::size_t, 4, 2>, std::layout_left>
    column_major{data};

// Mixed shape: only the dynamic dimension needs a runtime size.
std::mdspan<int, std::extents<std::size_t, 2, std::dynamic_extent>>
    mixed{data, cols};
```

The rank is always fixed at compile time, even when all sizes are dynamic.
With deduction, `std::mdspan view{data, 2, 4}` has **dynamic** extents: integer
literals passed to the constructor do not become static template extents.
Changing the layout changes how the view interprets the data; it does not
rearrange the underlying elements.

The standard `<mdspan>` header is required. A C++23 language switch alone is
not sufficient if the selected standard library does not implement it.

## What do I lean here?

- How to build multidimensional arrays in native c++