# `MyMat0`: a simple dense matrix class

This folder contains a small dense-matrix class used to discuss storage layout,
basic matrix operations, and the effect of memory access patterns on
performance.

The core class is [`MyMat0.hpp`](./MyMat0.hpp). It stores matrix entries in a
`std::vector<T>` and supports two storage policies:

- `ROWMAJOR`
- `COLUMNMAJOR`

The storage policy is a template parameter, so the choice is made at compile
time.

## Why this example is useful

The example shows how the same matrix interface can be implemented with
different internal memory layouts, while keeping the cost of the selection at
compile time instead of at run time.

This is useful because:

- indexing depends on the storage order
- extracting rows or columns has different costs depending on the layout
- matrix-vector and matrix-matrix products can be reorganized to be more cache
  friendly

## Tag dispatching

The class uses tag dispatching to select different implementations depending on
the storage policy. The tag type is built from the enum value:

```cpp
template <StoragePolicySwitch S>
using StorageType = std::integral_constant<StoragePolicySwitch, S>;
```

This tag is then used to select the appropriate overload for internal
operations such as:

- `getIndex()`
- `row()`
- `col()`
- `replaceRow()`
- `replaceCol()`

For example, the public `getIndex(i, j)` simply forwards to the correct
overload:

```cpp
constexpr size_type getIndex(size_type i, size_type j) const
{
  return getIndex(i, j, StorageType<storagePolicy>{});
}
```

In modern C++, some of these choices could also be expressed with
`if constexpr`. This example keeps tag dispatching because it is explicit,
compact, and still a useful technique to know.

## Files in this folder

- `MyMat0.hpp`
  The matrix class template.

- `MyMat0_util.hpp`
  Utility functions for matrix-matrix multiplication, including more
  cache-friendly variants.

- `MyMat0_util.cpp`
  BLAS-based implementations for `double`, enabled when BLAS support is
  available.

- `main_myMat0.cpp`
  A driver program that builds matrices, exercises the class, and compares the
  timings of different multiplication strategies.

- `Makefile`, `Makefile.inc`
  Build configuration for the example.

## Matrix multiplication

The folder contains several implementations of matrix-matrix multiplication.

### `matMul`

This is the straightforward triple-loop version. It is simple, but it does not
try to optimize memory access.

### `matMulOpt`

These overloads reorganize the computation depending on the storage layout of
the two matrices. The goal is to access data in contiguous memory whenever
possible, reducing cache misses.

There are four relevant cases:

1. row-major × row-major
2. row-major × column-major
3. column-major × column-major
4. column-major × row-major

Some cases are naturally efficient, while others require temporary copies or
less favorable access patterns.

### `matMulOptBlas`

When BLAS is enabled, the example also shows how to delegate the inner products
to an optimized external library. This is often the fastest solution in
practice for large matrices.

## What the main program demonstrates

The executable:

- creates small matrices and prints them
- computes a matrix-vector product
- evaluates a few matrix norms
- benchmarks different matrix-matrix multiplication strategies on larger
  matrices
- optionally compares the hand-written code with a BLAS-based implementation

For meaningful timings, compile in optimized mode:

```bash
make DEBUG=no
```

## BLAS note

If BLAS is available on your system, this example can use it. Check
[`Makefile.inc`](./Makefile.inc) and adapt the library path or library name if
needed.

If BLAS is not available, build with:

```bash
make BLAS=no
```

## What to learn from this folder

- how storage order affects matrix indexing
- how tag dispatching can select implementations at compile time
- why cache-friendly traversal matters for performance
- how the same algorithm may need different implementations for different data
  layouts
- how to connect simple custom code with BLAS routines
