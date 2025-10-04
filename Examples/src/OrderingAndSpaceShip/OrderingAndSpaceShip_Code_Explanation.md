# Ordering and Spaceship Operator in C++20

## Overview
The `main.cpp` file in the `OrderingAndSpaceShip` folder demonstrates the use of the C++20 spaceship operator (`<=>`) to define ordering relations for custom types. The spaceship operator simplifies the implementation of comparison operators (`<`, `<=`, `>`, `>=`, `==`, `!=`) by allowing them to be defined in one go. This file also explores the concepts of strong, weak, and partial ordering.

## Key Concepts and Classes

### 1. **Spaceship Operator (`<=>`)**
The spaceship operator is a three-way comparison operator introduced in C++20. It returns an ordering type that can be one of the following:
- `std::strong_ordering`: For types with a strict total order.
- `std::weak_ordering`: For types with a total order but not strict equality.
- `std::partial_ordering`: For types with a partial order.

### 2. **`Point` Class**
The `Point` class demonstrates the use of the defaulted spaceship operator. By defaulting the operator, all six relational operators (`<`, `<=`, `>`, `>=`, `==`, `!=`) are automatically generated with lexicographic ordering.

#### Code:
```cpp
struct Point
{
  double x = 0.0;
  double y = 0.0;
  friend auto operator<=>(const Point &, const Point &) = default;
};
```

#### Explanation:
- The default ordering is lexicographic (first `x`, then `y`).
- The return type is `std::strong_ordering` because `double` supports strict total ordering.

#### Example:
```cpp
Point a{4., 5.};
Point b{5., 6.};
std::cout << (a < b); // true
std::cout << (a == b); // false
```

### 3. **`Rational` Class**
The `Rational` class demonstrates weak ordering. Since the class does not normalize fractions, equivalent fractions (e.g., `1/2` and `2/4`) are not considered equal.

#### Code:
```cpp
struct Rational
{
  int den = 1;
  int num = 0;
  friend std::weak_ordering operator<=>(Rational const &, Rational const &);
  friend bool operator==(Rational const &, Rational const &);
};
```

#### Explanation:
- The spaceship operator returns `std::weak_ordering` because equivalent fractions are not strictly equal.
- The `==` operator is explicitly defined to use `std::is_eq` for consistency with `<=>`.

#### Example:
```cpp
Rational r1{1, 2};
Rational r2{2, 4};
std::cout << (r1 == r2); // true
std::cout << (r1 < r2); // false
```

### 4. **`Rational2` Class**
The `Rational2` class is similar to `Rational` but defaults the `==` operator. This means equality is based on the numerator and denominator being identical.

#### Code:
```cpp
struct Rational2
{
  int den = 1;
  int num = 0;
  friend std::weak_ordering operator<=>(Rational2 const &, Rational2 const &);
  friend bool operator==(Rational2 const &, Rational2 const &) = default;
};
```

#### Explanation:
- The `==` operator checks if both the numerator and denominator are equal.
- The spaceship operator still returns `std::weak_ordering`.

#### Example:
```cpp
Rational2 r1{1, 2};
Rational2 r2{2, 4};
std::cout << (r1 == r2); // false
std::cout << (r1 < r2); // false
```

### 5. **`Poset` Class**
The `Poset` class demonstrates partial ordering. Two `Poset` objects are only comparable if their `data` vectors have the same length.

#### Code:
```cpp
struct Poset
{
  std::vector<int> data;
  std::partial_ordering operator<=>(Poset const &);
};
```

#### Explanation:
- The spaceship operator returns `std::partial_ordering`.
- If the vectors have different lengths, the objects are unordered.
- Otherwise, lexicographic comparison is used.

#### Example:
```cpp
Poset ap{{1, 2, 3}};
Poset bp{{1, 2}};
std::cout << (ap < bp); // false
std::cout << std::is_eq(ap <=> bp); // unordered
```

## Examples and Outputs
The `main` function demonstrates the behavior of these classes:

1. **Points**:
   ```cpp
   Point a{4., 5.};
   Point b{5., 6.};
   std::cout << (a < b); // true
   std::cout << (a == b); // false
   ```

2. **Rationals**:
   ```cpp
   Rational r1{1, 2};
   Rational r2{2, 4};
   std::cout << (r1 == r2); // true
   std::cout << (r1 < r2); // false
   ```

3. **Posets**:
   ```cpp
   Poset ap{{1, 2, 3}};
   Poset bp{{1, 2}};
   std::cout << std::is_eq(ap <=> bp); // unordered
   ```

## C++20 Features Used
- **Spaceship Operator (`<=>`)**: Simplifies comparison operator definitions.
- **Ordering Types**: `std::strong_ordering`, `std::weak_ordering`, `std::partial_ordering`.
- **Ranges Library**: Used in `Poset` for vector comparison.
- **Defaulted Operators**: Simplifies equality operator definitions.

## Conclusion
The `OrderingAndSpaceShip` folder provides a comprehensive demonstration of the C++20 spaceship operator and its applications. By leveraging this operator, developers can simplify the implementation of comparison logic while ensuring consistency and clarity.