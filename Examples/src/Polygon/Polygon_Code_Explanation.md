# Polygon Module: Detailed Documentation

This document explains the structure and functionality of the C++ code in the `Polygon` folder. It covers the namespace organization, classes, methods, free functions, and utility helpers.

- Namespace: `Geometry`
- Source files:
  - `Polygon.hpp` (declarations and some inline implementations)
  - `Polygon.cpp` (implementations)
  - `PolygonUtilities.hpp` (factory function)
  - `main_poly.cpp` (usage example)

## 1) Namespace `Geometry`

All classes and functions are defined under the `Geometry` namespace to avoid global symbol collisions and to logically group geometry-related features.

## 2) Class `Point2D`

Represents a 2D point or vector in R².

- Data: `std::array<double,2> coor;` (private)
- Constructors:
  - `Point2D(double xx=0.0, double yy=0.0)` — initialize with coordinates
- Conversions and accessors:
  - `operator std::array<double,2>() const` — explicit conversion to array
  - `std::array<double,2> get() const` — copy of coordinates
  - `void set(double const& xx, double const& yy)` — set coordinates
  - `double x() const`, `double y() const` — coordinate accessors
- Operators (friends, inline in header):
  - `Point2D operator-(Point2D const& a, Point2D const& b)` — vector difference
  - `Point2D operator+(Point2D const& a, Point2D const& b)` — vector sum
  - `std::ostream& operator<<(std::ostream&, Point2D const&)` — print as `(x, y)` with newline

Alias: `using R2Vector = Point2D;`

## 3) Free Functions on Points/Vectors

- `double distance(Point2D const& a, Point2D const& b);`  
  Returns Euclidean distance, implemented via `norm(a - b)`.

- `double norm(R2Vector const& a);`  
  Euclidean norm (length) of a vector: `sqrt(x² + y²)`.

- `double sinAngle(R2Vector const& u, R2Vector const& v);`  
  Sine of the angle between vectors: `(ux*vy - uy*vx) / (|u||v|)`, i.e., 2D cross-product magnitude normalized.

- `using Vertices = std::vector<Point2D>;`  
  Container type alias for polygon vertices.

## 4) Abstract Base Class: `AbstractPolygon`

Defines the common interface and behavior for polygon-like shapes.

- Data members:
  - `Vertices vertexes;` — list of vertices
  - `bool isconvex = false;` — convexity flag

- Constructors:
  - `explicit AbstractPolygon(std::size_t numVertices);` — allocate `numVertices` zeroed points
  - `AbstractPolygon(Vertices const& v, bool checkConvex = true);` — set vertices and optionally check convexity
  - `AbstractPolygon() = default;` — default; derived classes must set vertices
  - Virtual destructor: `virtual ~AbstractPolygon() = default;`

- Core interface:
  - `virtual std::size_t size() const;` — number of vertices
  - `bool isConvex() const;` — convexity query
  - Vertex accessors (const and non-const):
    - `Point2D vertex(std::size_t i) const;`
    - `Point2D& vertex(std::size_t i);`
    - `Point2D operator[](std::size_t i) const;`
    - `Point2D& operator[](std::size_t i);`
  - `virtual void setVertexes(Vertices const& v);` — replace vertices and re-check convexity
  - Iteration support: `begin()`, `end()`, `cbegin()`, `cend()` returning STL iterators over `vertexes`
  - `virtual bool checkConvexity();` — compute and set `isconvex` using sign consistency of turning angles. Treats ≤3 vertices as convex
  - `virtual std::ostream& showMe(std::ostream& out=std::cout) const;` — print vertices and convexity info
  - `virtual double area() const = 0;` — pure virtual signed area

- Cloning support for polymorphic copy:
  - `virtual std::unique_ptr<AbstractPolygon> clone() = 0;` (protected)

### Convexity Check Details
The algorithm computes the sine of the angle between consecutive edges using `sinAngle`, walking around the polygon. If the sign of the sine changes (beyond an epsilon tolerance), the polygon is non-convex. Segments and triangles (≤3 vertices) are treated as convex.

## 5) Concrete Class: `Polygon` (generic)

Represents a generic polygon with arbitrary number of vertices.

- Inherits: `AbstractPolygon` (public, final)
- Constructors: inherits all from `AbstractPolygon` via `using AbstractPolygon::AbstractPolygon;`
- Overrides:
  - `double area() const override;` — computes signed area using a divergence-theorem-inspired discrete formula:  
    `A = 0.5 * sum_i ( x_i * (y_{i+1} - y_{i-1}) )`
  - `std::ostream& showMe(std::ostream& out) const override;` — prints header then defers to base `showMe`
- Cloning: `clone()` returns `std::make_unique<Polygon>(*this)`

## 6) Concrete Class: `Square`

Represents a square; enforces exactly 4 vertices and right-angle/side constraints.

- Inherits: `AbstractPolygon` (public, final)
- Constructors:
  - `Square(Vertices const& v);` — sets vertices and validates square (convex)
  - `Square()` — default 4 zero vertices, sets convex
  - `Square(Point2D origin, double length, double angle=0.0);` — parametric constructor; builds rotated square
- Overrides:
  - `std::size_t size() const override;` — always returns `4`
  - `bool checkConvexity() override;` — always `true` (square is convex by definition)
  - `double area() const override;` — signed area via 2D cross product of two adjacent sides
  - `void setVertexes(Vertices const& v) override;` — sets and validates square
  - `std::ostream& showMe(std::ostream& out) const override;` — header + base `showMe`
- Cloning: `clone()` returns `std::make_unique<Square>(*this)`
- Private helpers:
  - `void checkSquare();` — validation: requires 4 vertices and verifies `|area| ≈ l1*l2` with tolerance (100*epsilon) where `l1=|v1-v0|`, `l2=|v2-v3|`
- Constant:
  - `static constexpr std::size_t nVertices = 4;`

## 7) Concrete Class: `Triangle`

Represents a triangle; enforces exactly 3 vertices.

- Inherits: `AbstractPolygon` (public, final)
- Constructors:
  - `Triangle(Vertices const& v);` — sets vertices and validates
  - `Triangle()` — default 3 zero vertices; sets convex
- Overrides:
  - `std::size_t size() const override;` — always `3`
  - `bool checkConvexity() override;` — always `true`
  - `double area() const override;` — signed area `0.5 * (v × w)` using edges from vertex 0
  - `void setVertexes(Vertices const& v) override;` — sets and validates triangle
  - `std::ostream& showMe(std::ostream& out) const override;` — header + base `showMe`
- Cloning: `clone()` returns `std::make_unique<Triangle>(*this)`
- Private helpers:
  - `void checkTriangle();` — requires exactly 3 vertices
- Constant:
  - `static constexpr std::size_t nVertices = 3;`

## 8) Utility: `createPolygon` Factory

Defined in `PolygonUtilities.hpp`:

```cpp
template <typename... Args>
std::unique_ptr<AbstractPolygon>
createPolygon(std::string const& name, Args&&... args)
```

- Returns a `std::unique_ptr<AbstractPolygon>` to a newly constructed concrete polygon based on `name`:
  - "Polygon" → `Polygon`
  - "Square" → `Square`
  - "Triangle" → `Triangle`
- Forwards constructor arguments to the selected type (perfect forwarding)
- Returns `nullptr` for unknown names

## 9) Example Usage (`main_poly.cpp`)

Demonstrates creating and using `Polygon`, `Triangle`, and `Square` directly and via the factory:
- Build a 5-vertex `Polygon`, compute and print area
- Create `Triangle` from first 3 vertices, demonstrate polymorphism via base class pointer/reference
- Create unit `Square` with origin and side length; copy-construct
- Iterate over triangle vertices with range-based for
- Use `createPolygon("Polygon", v)` and `createPolygon("Square", q)` to construct via factory

## 10) Design Notes and Choices

- Polymorphic base class provides a common interface (`area`, `showMe`, `checkConvexity`)
- Signed area conventions: positive for counterclockwise orientation
- Convexity check uses numerical tolerance to handle floating-point errors
- Concrete shapes (`Square`, `Triangle`) hardcode vertex count and validation for safety
- Factory function provides simple decoupled construction based on runtime string

## 11) Quick API Reference

- Types: `Point2D`, `R2Vector`, `Vertices`
- Free functions: `distance`, `norm`, `sinAngle`
- Base class: `AbstractPolygon`
  - Core: `size()`, `area()`, `isConvex()`, `setVertexes()`, `checkConvexity()`, `showMe()`
  - Access: `vertex(i)`, `operator[](i)`, range iterators
- Derived classes: `Polygon`, `Square`, `Triangle` with overrides above
- Factory: `createPolygon(name, args...)` → `unique_ptr<AbstractPolygon>`

This module provides a clean example of an OO design with polymorphism for geometric entities, showcases modern C++ features (range-based loops, perfect forwarding, `unique_ptr`), and includes validation logic in shape-specific classes to ensure consistency and correctness.
