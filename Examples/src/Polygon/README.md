# Polygon: Polymorphism and Geometry Checks

This folder provides a small C++20 example of OOP polymorphism for 2D polygons.

## What is included
- `Polygon.hpp` / `Polygon.cpp`: geometry types and implementations.
- `PolygonUtilities.hpp`: simple factory (`createPolygon`).
- `main_poly.cpp`: usage example.

## Main ideas
- `AbstractPolygon` defines the common polymorphic interface.
- `Polygon` is a generic N-vertex polygon.
- `Triangle` and `Square` are specialized final classes with shape validation.
- Signed area is used (`> 0` for counterclockwise ordering, `< 0` for clockwise).

## Correctness notes
- Convexity is checked via consistent sign of edge cross products.
- Degenerate edges (zero length) are handled safely in angle/convexity helpers.
- `Square` validation checks:
  - exactly 4 vertices
  - non-degenerate edges
  - equal side lengths
  - orthogonality of consecutive sides

## Modern C++ style used
- `std::unique_ptr` for polymorphic ownership.
- `std::string_view` + perfect forwarding in factory.
- `[[nodiscard]]` on geometric query functions.
- const-correct iterators and `override` for virtual methods.

## Build and run
```bash
cd Examples/src/Polygon
make
./main_poly
```

## What you learn
- Public inheritance and runtime polymorphism.
- Basic geometric predicates (convexity and area).
- Factory-based runtime object creation.
