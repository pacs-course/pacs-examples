# `MyMat0_TypeTraitAndView`

This folder extends the `MyMat0` example with a simple view class,
[`TransposeView`](./MyMat0_views.hpp), which exposes the transpose of an
existing matrix without building a second matrix.

## Main idea

A view does not own data. It stores a reference to an existing object and
reinterprets its interface.

In this case:

- the underlying matrix keeps the original storage
- the view swaps the meaning of rows and columns
- element access `view(i,j)` is mapped to `matrix(j,i)`
- norms and matrix-vector multiplication are adapted to the transposed
  semantics

This is useful because the transpose can be accessed with no extra allocation
and no explicit copy.

## Files

- `MyMat0_views.hpp`
  Implements `TransposeView`.

- `main_myMat0.cpp`
  Demonstrates the view on mutable and constant matrices.

- `Makefile`, `Makefile.inc`
  Build configuration for the example.

## What the view supports

The transpose view provides:

- `nrow()` and `ncol()` with swapped dimensions
- element access through `operator()`
- `norm1()`, `normInf()`, and `normF()`
- matrix-vector multiplication
- formatted printing with `showMe()`
- mutation of the underlying matrix when the viewed matrix is non-const

The non-const element accessor is enabled with a type-trait-based condition, so
it is not available when the view is built from a constant matrix.

## Why type traits are used

The class uses `std::enable_if_t` together with `std::is_const_v` to expose the
non-const `operator()` only when the underlying matrix type is not `const`.

That means:

- `TransposeView<MyMat0<int>>` allows assignment through the view
- `TransposeView<const MyMat0<int>>` is read-only

## What to learn from this folder

- how to implement a lightweight transpose view
- how delegation can be used to adapt an existing interface
- how type traits can constrain part of a class interface
- how storage layout still matters even when working through a view
