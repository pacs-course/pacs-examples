# mathUtils.hpp - Generic Mathematical Utilities Library

This header file provides a comprehensive mathematical utilities library that implements generic algorithms for linear algebra operations across different data types. The design demonstrates sophisticated use of modern C++ features to create a unified interface that works seamlessly with scalars, complex numbers, Eigen matrices/vectors, and standard containers.

## Core Design Philosophy

The library follows a **type-generic approach** using C++20 concepts and compile-time branching with `if constexpr` to provide optimal implementations for different data types. This allows a single interface to automatically dispatch to the most efficient algorithm based on the input type, whether it's:

- Leveraging Eigen's optimized routines for matrices
- Handling complex number conjugation correctly
- Using parallel algorithms for standard containers

The design demonstrates **zero-cost abstractions** where the generic interface compiles down to the same performance as hand-written type-specific code.

## The Inner Product Foundation

At the heart of the library lies the `innerProduct` struct, which implements the mathematical concept of an inner product that correctly handles complex numbers:

```cpp
template <class T>
struct innerProduct {
    auto operator()(T const &a, T const &b) {
        if constexpr(apsc::TypeTraits::is_complex_v<T>)
            return a * std::conj(b);  // Complex conjugate for proper inner product
        else
            return a * b;             // Standard multiplication for real numbers
    }
};
```

### Why This Matters

For complex types, it computes `a * conj(b)` (where `conj` is the complex conjugate), while for real numbers it simply performs `a * b`. This distinction is **crucial for numerical correctness** in linear algebra operations involving complex numbers, as the proper inner product in complex vector spaces requires conjugation of one operand.

## Unified Dot Product Implementation

The `dot` function serves as the **central hub** that dispatches to appropriate implementations based on type traits:

### Type-Based Dispatch Strategy

```cpp
template <class T>
decltype(auto) dot(T const &a, T const &b) {
    if constexpr(apsc::TypeTraits::is_complex_v<T> || std::is_arithmetic_v<T>)
        return innerProduct<T>{}(a, b);           // Scalars and complex numbers
    else if constexpr(apsc::TypeTraits::is_eigen_v<T>)
        return a.dot(b);                          // Eigen's optimized method
    else {
        // Generic container implementation with parallel support
        // ...
    }
}
```

### Parallel Computing Strategy

The library showcases **dual parallelization approaches**:

#### Standard Library Approach (C++17)
```cpp
#ifndef _OPENMP
return std::transform_reduce(
    std::execution::par, std::begin(a), std::end(a), std::begin(b),
    static_cast<product_type>(0), std::plus<product_type>(),
    innerProduct<value_type>());
#endif
```

#### OpenMP Manual Implementation
```cpp
#ifdef _OPENMP
product_type res = 0;
#pragma omp parallel {
    product_type local_res = 0;
    #pragma omp for nowait
    for(std::size_t i = 0u; i < a.size(); ++i) {
        local_res += prod(a[i], b[i]);
    }
    #pragma omp critical {
        res += local_res;
    }
}
#endif
```

### Why Two Approaches?

- **Standard library version**: Portable, automatic parallelization with `std::execution::par`
- **OpenMP version**: Better performance, handles complex types (OpenMP's built-in reduction doesn't support `std::complex`)

## Norm Calculations and Specializations

### Function Overloading Pattern

The library demonstrates **elegant function overloading** where generic templates provide fallback implementations while specialized versions offer optimal performance:

```cpp
// Generic implementation using dot product
template <typename T>
decltype(auto) squaredNorm(T const &a) {
    return dot(a, a);  // Works for any type with defined dot product
}

// Specialized for Eigen types - uses native optimized method
template <apsc::TypeTraits::EigenMatrixType T>
decltype(auto) squaredNorm(const T &a) {
    return a.squaredNorm();  // Eigen's optimized implementation
}
```

### Norm Hierarchy

The library builds a **hierarchy of norms**:

1. **`squaredNorm`**: Foundation - most efficient, avoids square root
2. **`norm2s`**: Alias for `squaredNorm` - alternative naming
3. **`norm2`**: Standard Euclidean norm - `sqrt(squaredNorm(a))`
4. **`normInf`**: Infinity norm - maximum absolute value

### Infinity Norm Implementation

```cpp
template <class T>
decltype(auto) normInf(const T &a) {
    if constexpr(apsc::TypeTraits::is_complex_v<T> || std::is_arithmetic_v<T>)
        return std::abs(a);                           // Scalars
    else if constexpr(apsc::TypeTraits::is_eigen_v<T>)
        return a.template lpNorm<Eigen::Infinity>(); // Eigen's Lp norm
    else {
        // Parallel max_element for containers
        return std::abs(*std::max_element(std::execution::par, ...));
    }
}
```

## Vector Operations Namespace

The `vectorOperators` namespace **extends `std::vector`** with mathematical operations that mirror those available in specialized libraries like Eigen:

### Arithmetic Operations
```cpp
// Vector addition with parallel support
template <class T>
std::vector<T> operator+(std::vector<T> const &a, std::vector<T> const &b);

// Vector subtraction with parallel support  
template <typename T>
std::vector<T> operator-(std::vector<T> const &a, std::vector<T> const &b);

// Scalar multiplication (both pre and post)
template <typename T>
std::vector<T> operator*(T const &a, std::vector<T> const &b);
```

### Code Reuse Strategy
The post-multiplication operator demonstrates **elegant code reuse**:
```cpp
template <typename T>
std::vector<T> operator*(std::vector<T> const &b, T const &a) {
    return a * b;  // No code duplication - delegates to pre-multiplication
}
```

### Pretty Printing
```cpp
template <typename T>
std::ostream& operator<<(std::ostream &out, std::vector<T> const &v) {
    out << "[";
    if(v.size() != 0) {
        std::copy(v.begin(), v.end() - 1, std::ostream_iterator<T>(out, " "));
        out << v.back();
    }
    out << "]";
    return out;
}
```

## Type Safety and Concepts

The library extensively uses **C++20 concepts** for compile-time type checking:

```cpp
template <class T>
requires apsc::TypeTraits::EigenMatrixType<T> ||
         apsc::TypeTraits::ExtendedArithmetic<T> ||
         std::ranges::random_access_range<T>
decltype(auto) dot(T const &a, T const &b);
```

### Benefits of Concept-Based Design

1. **Clear interface contracts**: Functions only accept appropriate types
2. **Better error messages**: Concept failures provide meaningful diagnostics  
3. **Self-documenting code**: Requirements are explicit in the interface
4. **Compile-time optimization**: No runtime type checking overhead

## Performance Considerations

### Conditional Compilation Strategy

```cpp
#ifndef _OPENMP
// Use C++ standard library parallel algorithms
std::transform(std::execution::par, ...);
#else  
// Use OpenMP for better performance and complex type support
#pragma omp parallel for
#endif
```

### Memory Access Patterns

The library carefully considers **cache efficiency**:
- **OpenMP version**: Often more cache-friendly due to manual loop control
- **Standard library version**: Relies on implementation optimization
- **Eigen integration**: Leverages Eigen's vectorization and cache optimization

## Distance Calculations

The library provides **squared distance utilities** for both Eigen and standard vectors:

```cpp
// Eigen version - leverages expression templates
template <typename Derived, typename OtherDerived>
decltype(auto) squaredDistance(const Eigen::MatrixBase<Derived> &a,
                              Eigen::MatrixBase<OtherDerived> const &b) {
    return (a - b).squaredNorm();  // Eigen's optimized computation
}

// Standard vector version - uses our vector operators
template <typename T>
auto squaredDistance(std::vector<T> const &a, std::vector<T> const &b) {
    return squaredNorm(vectorOperators::operator-(a, b));
}
```

## Key Design Patterns

### 1. **Compile-Time Dispatch**
Using `if constexpr` to select optimal algorithms at compile time without runtime overhead.

### 2. **SFINAE and Concepts**
Ensuring type safety and providing clear error messages through concept constraints.

### 3. **Generic Programming**
Single interface supporting multiple backends (Eigen, standard containers, scalars).

### 4. **Zero-Cost Abstractions**
High-level interface that compiles to optimal low-level code.

### 5. **Parallel-First Design**
Built-in parallelization support with fallback to sequential execution.

## Use Cases

This library is particularly valuable for:

- **Scientific computing**: Unified interface across different mathematical libraries
- **High-performance computing**: Parallel algorithms with OpenMP and standard library support
- **Generic algorithms**: Code that works with both Eigen and standard containers
- **Complex number processing**: Correct mathematical operations for complex arithmetic
- **Educational purposes**: Demonstrates modern C++ design patterns and parallel programming

## Conclusion

The `mathUtils.hpp` library represents a **sophisticated example of generic programming** that balances flexibility, performance, and correctness across a wide range of mathematical computing scenarios. It demonstrates how modern C++ features can be used to create elegant, efficient, and type-safe mathematical software that adapts automatically to different data types and computing environments.