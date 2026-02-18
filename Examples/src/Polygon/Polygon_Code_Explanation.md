# Polygon Module: Code Explanation (C++20)

## Overview
`Geometry` models 2D polygons with a polymorphic hierarchy.

Files:
- `Polygon.hpp`: declarations and inline utilities
- `Polygon.cpp`: implementations
- `PolygonUtilities.hpp`: factory helper
- `main_poly.cpp`: demonstration

## Class structure
- `Point2D`: small 2D point/vector type.
- `AbstractPolygon`: base class with virtual API (`area`, `showMe`, `checkConvexity`, `setVertexes`).
- `Polygon`: generic polygon with signed-area computation.
- `Square`: fixed-size polygon (4 vertices) with square-specific validation.
- `Triangle`: fixed-size polygon (3 vertices).

## Geometry behavior
- `Polygon::area()` uses a signed shoelace-like formula.
- `Triangle::area()` and `Square::area()` use cross-product formulations.
- `AbstractPolygon::checkConvexity()` checks cross-product sign consistency over consecutive edges.

### Why convexity was revised
The convexity check now avoids dividing by vector norms. This is safer when edges are nearly zero length and avoids numerical instability from angle normalization.

## Square validation logic
`Square::checkSquare()` validates that vertices define a true square:
1. exactly 4 vertices
2. all edge lengths are positive (non-degenerate)
3. all four sides have equal length (within tolerance)
4. consecutive edges are orthogonal (dot product near zero)

This is stricter and more robust than checking only area/side ratios.

## Factory
`createPolygon(name, args...)` in `PolygonUtilities.hpp`:
- accepts `std::string_view`
- forwards constructor args to concrete types (`Polygon`, `Square`, `Triangle`)
- returns `std::unique_ptr<AbstractPolygon>`
- returns `nullptr` for unknown names

## C++20 style updates
- `[[nodiscard]]` on key query functions (`area`, `size`, `isConvex`, helpers).
- const overloads for `begin()` / `end()`.
- defaulted destructor where appropriate (`~Polygon() = default`).
- tighter const-correctness (`clone() const`).

## Runtime example
`main_poly.cpp` shows:
- direct construction of `Polygon`, `Triangle`, `Square`
- polymorphic access through `AbstractPolygon*` and reference
- range iteration on vertices
- factory usage with null checks
