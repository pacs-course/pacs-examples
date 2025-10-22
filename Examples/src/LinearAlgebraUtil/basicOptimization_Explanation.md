# Basic Optimization Utilities

## Overview
The `basicOptimization.hpp` file provides implementations of optimization algorithms for finding local and global minima of functions. These algorithms are designed for continuous functions and leverage modern C++ features such as concepts and the ranges library. The file includes:

- Golden section search for unimodal functions.
- Bracketing methods to identify intervals containing minima.
- Brent's method for both local and global minimization.

## Key Functions

### 1. `golden_search`
This function implements the golden section search algorithm to find the extremum of a unimodal function within a specified interval.

#### Template Parameters:
- `Function`: A callable object with the signature `double(double)` or `double(const double&)`.

#### Parameters:
- `f`: The function to minimize.
- `a`, `b`: The endpoints of the interval.
- `tol`: Desired tolerance for the result.
- `maxIter`: Maximum number of iterations to safeguard against non-convergence.

#### Returns:
- A tuple containing the estimated extremum and a boolean indicating convergence.

#### Example:
```cpp
auto [min, converged] = golden_search([](double x) { return x * x; }, -1.0, 1.0);
```

### 2. `bracketIntervalMinimum`
This function attempts to find an interval containing a minimum of a continuous function.

#### Template Parameters:
- `Function`: A callable object with the signature `double(double)`.

#### Parameters:
- `f`: The function to analyze.
- `x1`: Initial guess for the minimum.
- `h`: Initial increment.
- `maxIter`: Maximum number of iterations.

#### Returns:
- A tuple containing the bracket points and a status code:
  - `0`: Success.
  - `1`: Bad initial point.
  - `2`: Maximum iterations exceeded.

#### Example:
```cpp
auto [x1, x2, status] = bracketIntervalMinimum([](double x) { return x * x; }, 0.0);
```

### 3. `Brent_glomin`
This function implements Brent's method for global minimization of a function over a specified interval.

#### Parameters:
- `a`, `b`: Endpoints of the interval.
- `c`: Initial guess for the global minimizer.
- `m`: Estimate of the bound on the second derivative.
- `e`: Absolute error tolerance for function evaluation.
- `t`: Absolute error tolerance for the result.
- `f`: The function to minimize.

#### Returns:
- A tuple containing the estimated global minimizer and the function value at that point.

#### Example:
```cpp
auto [x, fx] = Brent_glomin(0.0, 1.0, 0.5, 1.0, 1e-5, 1e-5, [](double x) { return x * x; });
```

### 4. `Brent_local_min`
This function implements Brent's method for local minimization of a function over a specified interval.

#### Parameters:
- `a`, `b`: Endpoints of the interval.
- `t`: Absolute error tolerance for the result.
- `f`: The function to minimize.

#### Returns:
- A tuple containing the estimated local minimizer and the function value at that point.

#### Example:
```cpp
auto [x, fx] = Brent_local_min(0.0, 1.0, 1e-5, [](double x) { return x * x; });
```

## C++ Features Used

### 1. **Concepts**
The file uses concepts to constrain the template parameter for functions. The `TypeTraits::ScalarFunction` concept ensures that the provided callable objects have the correct signature.

### 2. **`std::numbers`**
The `std::numbers` library is used to access mathematical constants like the golden ratio.

### 3. **Modern C++ Style**
The code adopts modern C++ practices, including:
- Use of `constexpr` for compile-time constants.
- Structured bindings for returning multiple values.
- Use of `std::tuple` and `std::array` for returning results.

## Usage Notes

1. **Function Requirements**:
   - The functions must be continuous and differentiable.
   - For `Brent_glomin`, the second derivative must be bounded.

2. **Convergence**:
   - The algorithms assume that the minimum lies within the specified interval.
   - No explicit convergence guarantees are provided for `Brent_glomin`.

3. **Performance**:
   - The algorithms are efficient for unimodal functions.
   - For non-unimodal functions, results may vary.

## Conclusion
The `basicOptimization.hpp` file provides robust and efficient implementations of optimization algorithms, leveraging modern C++ features for clarity and performance. These utilities are suitable for a wide range of applications requiring function minimization.