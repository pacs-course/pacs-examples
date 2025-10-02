# Basic Zero-Finding Utilities

## Overview
The `basicZeroFun.hpp` file provides implementations of numerical methods for finding zeros of scalar functions. These methods are designed for continuous functions and include classical root-finding algorithms such as:

- Regula Falsi
- Bisection
- Secant
- Newton
- Brent's method

The file also includes a utility for bracketing intervals that contain zeros. Modern C++ features, such as concepts, are used to ensure type safety and flexibility.

## Key Functions

### 1. `regulaFalsi`
This function implements the Regula Falsi method, a bracketing method for finding zeros of a function.

#### Template Parameters:
- `Function`: A callable object with the signature `double(double)` or `double(const double&)`.

#### Parameters:
- `f`: The function to find the zero of.
- `a`, `b`: The endpoints of the initial interval.
- `tol`: Relative tolerance.
- `tola`: Absolute tolerance.

#### Returns:
- The approximated zero of the function.

#### Example:
```cpp
double root = regulaFalsi([](double x) { return x * x - 2; }, 0.0, 2.0);
```

### 2. `bisection`
This function implements the Bisection method, a robust bracketing method for finding zeros.

#### Parameters:
- `f`: The function to find the zero of.
- `a`, `b`: The endpoints of the initial interval.
- `tol`: Tolerance for the result.

#### Returns:
- The approximated zero of the function.

#### Example:
```cpp
double root = bisection([](double x) { return x * x - 2; }, 0.0, 2.0);
```

### 3. `secant`
This function implements the Secant method, which uses linear interpolation to find zeros.

#### Parameters:
- `f`: The function to find the zero of.
- `a`, `b`: Initial points for the method.
- `tol`: Relative tolerance.
- `tola`: Absolute tolerance.
- `maxIt`: Maximum number of iterations.

#### Returns:
- A tuple containing the approximated zero and a boolean indicating convergence.

#### Example:
```cpp
auto [root, converged] = secant([](double x) { return x * x - 2; }, 0.0, 2.0);
```

### 4. `Newton`
This function implements Newton's method, which uses the derivative of the function to find zeros.

#### Parameters:
- `f`: The function to find the zero of.
- `df`: The derivative of the function.
- `a`: Initial guess for the zero.
- `tol`: Relative tolerance.
- `tola`: Absolute tolerance.
- `maxIt`: Maximum number of iterations.

#### Returns:
- A tuple containing the approximated zero and a boolean indicating convergence.

#### Example:
```cpp
auto [root, converged] = Newton([](double x) { return x * x - 2; }, [](double x) { return 2 * x; }, 1.0);
```

### 5. `bracketInterval`
This function attempts to find an interval that brackets a zero of a function.

#### Parameters:
- `f`: The function to analyze.
- `x1`: Initial point.
- `h`: Initial increment for sampling.
- `maxIter`: Maximum number of iterations.

#### Returns:
- A tuple containing the bracketing points and a boolean indicating success.

#### Example:
```cpp
auto [x1, x2, success] = bracketInterval([](double x) { return x * x - 2; }, 1.0);
```

### 6. `brent_search`
This function implements Brent's method, a combination of bisection, secant, and inverse quadratic interpolation.

#### Parameters:
- `f`: The function to find the zero of.
- `a`, `b`: The endpoints of the bracketing interval.
- `tol`: Tolerance for the result.
- `maxIter`: Maximum number of iterations.

#### Returns:
- A tuple containing the approximated zero and a boolean indicating convergence.

#### Example:
```cpp
auto [root, converged] = brent_search([](double x) { return x * x - 2; }, 0.0, 2.0);
```

## C++ Features Used

### 1. **Concepts**
The file uses concepts to constrain the template parameters for functions. The `TypeTraits::ScalarFunction` concept ensures that the provided callable objects have the correct signature.

### 2. **Modern C++ Practices**
- Use of `std::tuple` for returning multiple values.
- Use of `std::swap` for efficient variable swapping.
- Assertions (`SURE_ASSERT`) to enforce preconditions.

## Usage Notes

1. **Function Requirements**:
   - The functions must be continuous.
   - For Newton's method, the derivative must be provided.

2. **Convergence**:
   - The bracketing methods (`regulaFalsi`, `bisection`, `brent_search`) guarantee convergence if the initial interval brackets a zero.
   - The open methods (`secant`, `Newton`) may fail to converge if the initial guesses are poor.

3. **Performance**:
   - Brent's method is generally efficient and robust.
   - Bisection is the most reliable (if the function is continuous and brackets a zero) but slower.
   - Newton's method can be very fast (quadratic convergence for C^2 functions) but requires a good initial guess and the derivative.

## Conclusion
The `basicZeroFun.hpp` file provides a comprehensive set of tools for zero-finding of univariate functions, leveraging modern C++ features for robustness and flexibility. These utilities are suitable for a wide range of applications requiring root-finding algorithms.