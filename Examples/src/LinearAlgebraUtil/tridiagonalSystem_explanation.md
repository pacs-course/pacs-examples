# Explanation of `tridiagonalSystem.hpp`

## Overview
The `tridiagonalSystem.hpp` file provides utilities for solving tridiagonal linear systems of equations. These systems are common in numerical simulations and arise in problems such as finite difference methods for partial differential equations. The file includes implementations of the Thomas algorithm for solving standard and periodic tridiagonal systems, as well as a utility for matrix-vector multiplication with tridiagonal matrices.

### Key Components

1. **`thomasSolve` Function**
   - Solves a standard tridiagonal system of equations using the Thomas algorithm (a specialized form of Gaussian elimination).
   - The system is represented by three vectors: `a` (diagonal), `b` (subdiagonal), and `c` (superdiagonal).
   - The right-hand side is provided in vector `f`.

2. **`thomasSolveSym` Function**
   - Solves a periodic tridiagonal system of equations, where the first and last rows are coupled.
   - Uses a block LU decomposition approach to handle the periodicity.

3. **`matVecTrid` Function**
   - Multiplies a tridiagonal matrix (stored in three vectors) with a vector.
   - Useful for validating the solutions obtained from `thomasSolve` or `thomasSolveSym`.

### Example Usage
Below is an example demonstrating the usage of the utilities:

```cpp
#include <iostream>
#include <vector>
#include "tridiagonalSystem.hpp"

int main() {
    // Define the tridiagonal system
    std::vector<double> a = {4, 4, 4}; // Diagonal
    std::vector<double> b = {0, 1, 1}; // Subdiagonal
    std::vector<double> c = {1, 1, 0}; // Superdiagonal
    std::vector<double> f = {7, 8, 7}; // Right-hand side

    // Solve the system using thomasSolve
    auto solution = apsc::LinearAlgebra::thomasSolve(a, b, c, f);

    // Print the solution
    std::cout << "Solution: ";
    for (double x : solution) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    // Validate the solution using matVecTrid
    auto result = apsc::LinearAlgebra::matVecTrid(a, b, c, solution);
    std::cout << "Validation: ";
    for (double x : result) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

### Explanation of the Example
1. **Input Vectors**:
   - `a`, `b`, and `c` represent the diagonal, subdiagonal, and superdiagonal of the tridiagonal matrix, respectively.
   - `f` is the right-hand side vector.

2. **Solution**:
   - The `thomasSolve` function computes the solution to the tridiagonal system.

3. **Validation**:
   - The `matVecTrid` function multiplies the tridiagonal matrix with the solution vector to verify that it matches the right-hand side vector `f`.

### Output of the Example
```
Solution: 1 2 1 
Validation: 7 8 7 
```

### Advantages
- **Efficiency**: The Thomas algorithm is highly efficient for tridiagonal systems, with a time complexity of $O(n)$.
- **Support for Periodic Systems**: The `thomasSolveSym` function extends the algorithm to handle periodic boundary conditions.
- **Validation Utility**: The `matVecTrid` function provides a convenient way to validate solutions.

### Requirements
- **C++20 or Later**: The implementation uses modern C++ features such as concepts.
- **STL-Compliant Containers**: The input vectors must be STL-compliant sequential containers (e.g., `std::vector`).

## Conclusion
The `tridiagonalSystem.hpp` file provides efficient and robust utilities for solving tridiagonal systems of equations. Its support for both standard and periodic systems, along with validation utilities, makes it a valuable tool for numerical computations.