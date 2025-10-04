# PointInSimplex: Code Overview and Usage Guide

This document explains the code in the `PointInSimplex` folder and shows how to use it from both C++ and Python.

## What it does

Utilities to determine if a point lies inside:
- a 2D triangle (2-simplex in R²)
- a 3D tetrahedron (3-simplex in R³)
- a 3D triangle (2-simplex embedded in R³), also reporting distance from the triangle plane

All functions return barycentric coordinates along with the inside/outside boolean. Barycentric coordinates are useful for interpolation and provide an immediate check (`all >= 0` for points inside, within tolerance).

## File layout

- `PointInSimplex.hpp` — header-only implementations of the geometry predicates
- `PointInS_Vector_Traits.hpp` — traits and adapters for underlying vector/matrix types (Eigen or std::array)
- `pyPointInSimplex.cpp` — pybind11 bindings for Python module `PointInSimplex`
- `main_pIS.cpp` — small interactive C++ demo
- `pyPIS_test.py` — small interactive Python demo
- `PointInSimplex.cpython-...so` — built Python extension module produced bt the compilation
- `Makefile` — build rules for C++ demo and Python extension

## Type traits and backends

The code can work with either Eigen or standard library arrays, selected at compile time:

- If `HAVE_EIGEN` is defined:
  - Points/vectors are Eigen types (e.g., `Eigen::Vector3d`)
  - Simplices are Eigen matrices (rows are vertices)
  - Helper functions `dot`, `norm`, `extract` are thin wrappers over Eigen

- Otherwise (no Eigen):
  - Points/vectors are `std::array<double, N>`
  - Simplices are arrays of points
  - Helper functions `dot`, `norm`, `extract` are implemented over std::array

All concrete code is templated over the traits `apsc::PointInS_Traits`:
- `Point2D`, `Vector2D`, `Point3D`, `Vector3D`, `Vector4D`
- `Triangle` (3×2), `Triangle3D` (3×3), `Tetrahedron` (4×3)
- Helpers: `dot`, `norm`, `cross`, `extract`

## Public API (C++)

All functions live in namespace `apsc` (in `PointInSimplex.hpp`). They return tuples typed with the traits’ vector types.

### 1) Point in 3D Tetrahedron

Signature:
```cpp
std::tuple<bool, PointInS_Traits::Vector4D>
pointInTetrahedron(const PointInS_Traits::Point3D& p,
                   const PointInS_Traits::Tetrahedron& t);
```
- Returns `(inside, lambdas)` where `lambdas = [λ0, λ1, λ2, λ3]` are barycentric coordinates w.r.t. the tetrahedron vertices.
- Algorithm: compute λ via signed volumes using triple products. Use a small negative tolerance `tol = -1e-12` to accept near-boundary cases.

### 2) Point in 2D Triangle

Signature:
```cpp
std::tuple<bool, PointInS_Traits::Vector3D>
pointInTriangle(const PointInS_Traits::Point2D& p,
                const PointInS_Traits::Triangle& t);
```
- Returns `(inside, lambdas)` where `lambdas = [λ0, λ1, λ2]`.
- Algorithm: compute λ via oriented areas using 2D cross products.

### 3) Point in 3D Triangle (+ plane info)

Signature:
```cpp
std::tuple<bool, bool, double, PointInS_Traits::Vector3D>
pointInTriangle3(const PointInS_Traits::Point3D& p,
                 const PointInS_Traits::Triangle3D& t,
                 double eps = 1.e-3);
```
- Returns `(inside, onPlane, distance, lambdas)`:
  - `inside`: true if barycentric coords of projection are all ≥ tol
  - `onPlane`: true if |signed distance| ≤ eps
  - `distance`: signed distance from point to triangle plane (positive along normal)
  - `lambdas`: barycentric coordinates of the point’s projection on the plane
- Algorithm: compute plane normal `n = v01 × v02`, signed distance `z = dot(n, s)/||n||`, then barycentric coordinates using cross/dot with `n`. Uses `tol = -1e-12` for inclusivity near edges.

## Implementation notes

- For all tests, barycentric coordinates satisfy `sum λi = 1`.
- A small negative tolerance (`-1e-12`) is used to robustly treat boundary cases in floating point.
- Degenerate simplices (zero area/volume) will lead to division by zero; no special handling is included—ensure valid input.

## C++ usage example

From `main_pIS.cpp` (Eigen backend shown):
```cpp
#include "PointInSimplex.hpp"

apsc::PointInS_Traits::Triangle t; // 2D triangle (3x2 if Eigen)
// Fill triangle vertices
// ...
apsc::PointInS_Traits::Point2D p; // fill point

auto [inside, lambda] = apsc::pointInTriangle(p, t);
if (inside) {
    // lambda[0], lambda[1], lambda[2]
}

apsc::PointInS_Traits::Tetrahedron tet; // 4x3
apsc::PointInS_Traits::Point3D p3;      // 3D point
auto [inTet, lambda4] = apsc::pointInTetrahedron(p3, tet);

apsc::PointInS_Traits::Triangle3D t3;   // 3x3 triangle in 3D
auto [inTri3, onPlane, dist, lambda3] = apsc::pointInTriangle3(p3, t3);
```

Build and run the C++ demo:
```bash
make main_pIS
./main_pIS
```

## Python module usage

The pybind11 module exposes the same functionality under module name `PointInSimplex`:

```python
import PointInSimplex as pis
import numpy as np

triangle = np.array([[0,0],[1,0],[0,1]], dtype=float)
point2d = np.array([0.2, 0.3], dtype=float)
inside2d, lambdas2d = pis.pointInTriangle(point2d, triangle)

# Tetrahedron
tet = np.array([[0,0,0],[1,0,0],[0,1,0],[0,0,1]], dtype=float)
point3d = np.array([0.1, 0.2, 0.1], dtype=float)
insideTet, lambdas4 = pis.pointInTetrahedron(point3d, tet)

# Triangle in 3D
tri3d = np.array([[0,0,0],[1,0,0],[0,1,0]], dtype=float)
inside, onplane, dist, lambdas3 = pis.pointInTriangle3(p=point3d, t=tri3d, eps=1e-3)
```

Run the provided test:
```bash
python pyPIS_test.py
```

Note: From your recent run, you saw an output like:
```
the point is in the triangle:  True  and the barycentric coordinates are:  False
```
This means you printed the second element (a vector) in a truthy context somewhere. In Python, a NumPy array prints as `False` when cast to bool. Make sure you print the whole tuple elements separately, as in the example above, or use `inside[1].tolist()` for readability.

## Building the Python module

- Requires `pybind11` and, optionally, `Eigen` for the Eigen backend.
- The provided `Makefile` has targets:
  - `make all` — builds C++ demo and Python extension
  - `make pyModule` — builds only Python extension
  - `make main_pIS` — builds only C++ demo

If you don’t have Eigen available, you can build with the std::array backend by not defining `HAVE_EIGEN` (check `Makefile.inc`).

## FAQ and tips

- Barycentric coords sum to 1 (within FP tolerance). For interpolation: `p ≈ Σ λi v_i`.
- On edges/vertices, one or more λ ≈ 0; due to tolerance, very small negatives are accepted.
- If you need strict interior-only tests, change `tol` to `0.0` in the functions.
- For performance-critical code, ensure Eigen is compiled with optimizations (e.g., `-O3`) and avoid dynamic allocations.

## Potential extensions

- Add degenerate-simplex checks (zero area/volume) with clear error handling
- Generalize to N-D simplices using matrix solves for barycentric coordinates
- Return also the projection point for `pointInTriangle3`
- Add vectorized batch interfaces (arrays of points)

---
This folder provides robust, minimal utilities to compute barycentric coordinates and inside/outside tests for common simplices, with both C++ and Python interfaces and a flexible type backend (Eigen or std::array).
