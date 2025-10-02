# Heat Exchange 1D Bar: Code Explanation

## Overview

This document explains the C++ implementation of a finite element solver for the 1D heat equation in a bar. The code solves the steady-state temperature distribution problem using linear finite elements with two different solution methods: Gauss-Seidel iterative solver and direct Thomas algorithm.

## Problem Description

### Physical Problem

The code models heat exchange in a one-dimensional bar of length $L$ with:
- Constant thermal conductivity $k$
- One end at fixed temperature $T_0$ (Dirichlet boundary condition)  
- Other end under adiabatic conditions (Neumann boundary condition: zero heat flux)
- Heat exchange with surrounding air at temperature $T_e$ through convection

### Mathematical Formulation

The steady-state temperature distribution satisfies:

$$-k \frac{d^2T}{dx^2} + h_p(T-T_e) = 0, \quad 0 < x < L$$

with boundary conditions:
$$T(0) = T_0, \quad \frac{dT}{dx}(L) = 0$$

where $h_p = \frac{2h(a_1+a_2)}{a_1 a_2}$ is the convective heat exchange coefficient per unit length.

### Normalized Form

Using the temperature difference $\theta = T - T_e$ and normalizing coordinates $x \rightarrow x/L$:

$$-\frac{d^2\theta}{dx^2} + a\theta = 0, \quad 0 < x < 1$$

$$\theta(0) = \theta_0 = T_0 - T_e, \quad \frac{d\theta}{dx}(1) = 0$$

where $a = \frac{2L^2 h(a_1+a_2)}{k a_1 a_2}$ is the dimensionless convection parameter.

### Analytical Solution

The exact solution is:
$$\theta(x) = \theta_0 \frac{\cosh[\sqrt{a}(1-x)]}{\cosh(\sqrt{a})}$$

## Code Architecture

### File Structure

- **`main.cpp`**: Main program implementing the finite element solver
- **`parameters.hpp`**: Defines the `parameters` struct with default values
- **`readParameters.hpp/cpp`**: Functions for reading parameters from files
- **`thomas.hpp`**: Template implementation of Thomas algorithm for tridiagonal systems
- **`parameters.pot`**: GetPot parameter file with problem configuration
- **`parameters.json`**: Alternative JSON parameter file

### Key Classes and Structures

#### `parameters` Struct

```cpp
struct parameters {
    int itermax = 1000000;    // Max iterations for Gauss-Seidel
    double toler = 1.e-8;     // Convergence tolerance
    double L = 40.0;          // Bar length [m]
    double a1 = 4.0;          // First cross-section dimension [m]
    double a2 = 50.0;         // Second cross-section dimension [m]
    double To = 46.0;         // Boundary temperature [°C]
    double Te = 20.0;         // External temperature [°C]
    double k = 0.164;         // Thermal conductivity [W/mK]
    double hc = 200e-6;       // Convection coefficient [W/m²K]
    int M = 100;              // Number of finite elements
    int solverType = 0;       // Solver choice (0: direct, 1: Gauss-Seidel)
};
```

## Main Program Flow

### 1. Parameter Processing

```cpp
GetPot cl(argc, argv);
```

The program uses the GetPot library for command-line argument processing and parameter file reading. It supports:
- `-h` or `--help`: Display help message
- `-v`: Verbose output
- `-p parameterFile`: Specify parameter file (default: `parameters.pot`)

The code automatically detects file format (GetPot vs JSON) based on file extension.

### 2. Parameter Reading

Two parameter reading functions are available:
- `readParameters()`: Reads GetPot format files
- `readParameters_json()`: Reads JSON format files

Both functions provide default values if the file is missing or parameters are not specified.

### 3. C++17 Structured Bindings

The code demonstrates modern C++ features with conditional compilation:

```cpp
#if __cplusplus < 201703L
    // Traditional parameter extraction
    const int &itermax = param.itermax;
    const double &toler = param.toler;
    // ... more parameters
#else
    // C++17 structured bindings
    const auto &[itermax, toler, L, a1, a2, To, Te, k, hc, M, solverType] = param;
#endif
```

### 4. Problem Setup

The code computes the dimensionless parameter:
```cpp
const auto act = 2. * (a1 + a2) * hc * L * L / (k * a1 * a2);
```

This corresponds to the parameter $a$ in the mathematical formulation.

## Solution Methods

### Method 1: Gauss-Seidel Iterative Solver

When `solverType == 1`, the code uses Gauss-Seidel iterations:

```cpp
// Initialize with linear temperature variation
for(int m = 0; m <= M; ++m)
    theta[m] = (1. - m * h) * (To - Te) / Te;

// Gauss-Seidel iterations
do {
    epsilon = 0.;
    // Interior nodes: i = 1, ..., M-1
    for(int m = 1; m < M; m++) {
        xnew = (theta[m - 1] + theta[m + 1]) / (2. + h * h * act);
        epsilon += (xnew - theta[m]) * (xnew - theta[m]);
        theta[m] = xnew;
    }
    // Boundary node with Neumann condition
    xnew = theta[M - 1];
    epsilon += (xnew - theta[M]) * (xnew - theta[M]);
    theta[M] = xnew;
    
    iter++;
} while((sqrt(epsilon) > toler) && (iter < itermax));
```

**Algorithm Details:**
- **Initialization**: Linear temperature profile from boundary to adiabatic end
- **Update Formula**: For interior nodes: $\theta_i^{(k+1)} = \frac{\theta_{i-1}^{(k)} + \theta_{i+1}^{(k)}}{2 + h^2 a}$
- **Boundary Condition**: Neumann BC: $\theta_M^{(k+1)} = \theta_{M-1}^{(k)}$
- **Convergence**: $\|\theta^{(k+1)} - \theta^{(k)}\|_2 \leq \text{tolerance}$

### Method 2: Direct Solution with Thomas Algorithm

When `solverType != 1`, the code uses the Thomas algorithm for tridiagonal systems:

```cpp
// Build tridiagonal matrix components
std::vector<double> a(M + 1, 1.);                    // Main diagonal
std::vector<double> b(M + 1, -1. / (2. + h * h * act)); // Sub-diagonal
std::vector<double> c = b;                            // Super-diagonal (symmetric)

// Boundary condition corrections
b.back() = -1;      // Neumann BC at x = L
c.front() = 0;      // Dirichlet BC at x = 0

// Right-hand side
std::vector<double> source(M + 1, 0.);
source.front() = (To - Te) / Te;  // Dirichlet BC contribution

// Solve using Thomas algorithm
theta = apsc::thomasSolve(a, b, c, source);
```

**Thomas Algorithm** (implemented in `thomas.hpp`):
- **Forward Sweep**: Eliminate sub-diagonal elements
- **Back Substitution**: Solve for unknowns from bottom to top
- **Complexity**: $O(n)$ for tridiagonal systems vs $O(n^3)$ for general matrices

## Modern C++ Features

### 1. Template Programming

The Thomas algorithm is implemented as a template:

```cpp
template <class C>
C thomasSolve(const C &a, const C &b, const C &c, const C &f)
```

This allows usage with any STL-compliant container (vector, array, etc.).

### 2. C++20 Format Library

Conditional compilation for modern formatting:

```cpp
#if __cplusplus > 201703L
    // C++20 std::format
    f << std::format("{:<18.15f}\t\t{:<18.15f}\t\t{:<18.15f}\n", 
                     m * h * L, theta[m], thetaa[m]);
#else
    // Traditional formatting
    f.setf(std::ios::left, std::ios::adjustfield);
    f.width(18);
    f.precision(15);
    f << m * h * L << "\t\t" << theta[m] << "\t\t" << thetaa[m] << "\n";
#endif
```

### 3. Range-based Loops

Modern iteration syntax:
```cpp
for(auto &t : theta)
    t = Te * (1 + t);  // Convert back to physical temperatures
```

### 4. Auto Type Deduction

Extensive use of `auto` for type inference:
```cpp
const auto act = 2. * (a1 + a2) * hc * L * L / (k * a1 * a2);
const auto h = 1. / M;
```

## Output and Visualization

### File Output

The program generates `result.dat` with three columns:
- Physical coordinate: $x_i \cdot L$
- Numerical solution: $\theta_h(x_i)$  
- Analytical solution: $\theta_{exact}(x_i)$

### Gnuplot Integration

Optional real-time plotting with gnuplot-iostream:

```cpp
#ifdef GNUPLOT
Gnuplot gp;
gp << "plot" << gp.file1d(std::tie(coor, theta)) << "w lp lw 2 title 'uh',"
   << gp.file1d(std::tie(coor, thetaa)) << "w l lw 2 title 'uex'" << std::endl;
#endif
```

## Error Handling and Robustness

### Parameter Validation

```cpp
if(param.M <= 0 || param.L <= 0 || param.k <= 0) {
    cerr << "Error: Invalid or missing parameters in input file." << endl;
    return 1;
}
```

### Convergence Monitoring

```cpp
if(iter < itermax)
    cout << "M=" << M << "  Convergence in " << iter << " iterations" << endl;
else {
    cerr << "NOT CONVERGING in " << itermax << " iterations "
         << "||dx||=" << sqrt(epsilon) << endl;
    status = 1;
}
```

## Physical Parameters

| Parameter | Symbol | Default Value | Units | Description |
|-----------|--------|---------------|-------|-------------|
| `L` | $L$ | 40.0 | m | Bar length |
| `a1` | $a_1$ | 4.0 | m | Cross-section dimension 1 |
| `a2` | $a_2$ | 50.0 | m | Cross-section dimension 2 |
| `To` | $T_0$ | 46.0 | °C | Boundary temperature |
| `Te` | $T_e$ | 20.0 | °C | External temperature |
| `k` | $k$ | 0.164 | W/mK | Thermal conductivity |
| `hc` | $h$ | 200×10⁻⁶ | W/m²K | Convection coefficient |
| `M` | $M$ | 100 | - | Number of elements |

## Compilation and Usage

### Prerequisites

- C++ compiler with C++17 support (recommended)
- GetPot library for parameter handling
- Thomas algorithm library (`thomas.hpp`)
- Optional: Boost libraries for gnuplot-iostream

### Build Commands

```bash
make main          # Compile the program
make DEBUG=no      # Optimized build
```

### Program Usage

```bash
./main [-h] [-v] [-p parameterFile]
```

- `-h`: Display help message
- `-v`: Verbose output with parameter values
- `-p file`: Specify parameter file (default: `parameters.pot`)

### Example Parameter File (GetPot format)

```
# Parameters for heat exchange simulation
L = 40.0           # Bar length [m]
a1 = 4.0          # Cross-section dimension 1 [m]  
a2 = 50.0         # Cross-section dimension 2 [m]
To = 46.0         # Boundary temperature [°C]
Te = 20.0         # External temperature [°C]
k = 0.164         # Thermal conductivity [W/mK]
hc = 200.0e-6     # Convection coefficient [W/m²K]
M = 100           # Number of finite elements
solverType = 0    # 0: Thomas algorithm, 1: Gauss-Seidel
itermax = 1000000 # Max iterations for Gauss-Seidel
toler = 1.0e-8    # Convergence tolerance
```

## Educational Value

This code demonstrates:

1. **Finite Element Method**: Basic 1D linear finite elements
2. **Numerical Linear Algebra**: Comparison of direct vs iterative solvers
3. **Modern C++**: C++17/20 features, templates, STL containers
4. **Software Engineering**: Modular design, parameter handling, error checking
5. **Scientific Computing**: Physical modeling, numerical methods, visualization
6. **Performance Considerations**: $O(n)$ vs $O(n^3)$ algorithms

## Possible Extensions

1. **Enhanced I/O**: User-configurable output filenames
2. **Advanced Solvers**: Integration with Eigen library, multiple solver options
3. **Transient Problems**: Time-dependent heat equation with time integration
4. **Variable Parameters**: Spatially varying material properties
5. **Higher-Order Elements**: Quadratic or cubic finite elements
6. **Nonlinear Problems**: Temperature-dependent material properties
7. **Mesh Refinement**: Adaptive mesh generation
8. **Parallel Processing**: OpenMP or MPI parallelization

This implementation provides a solid foundation for learning finite element methods and serves as a stepping stone to more complex computational physics applications.