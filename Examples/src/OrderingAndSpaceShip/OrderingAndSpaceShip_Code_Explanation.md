# Ordering and spaceship operator in C++20

This folder demonstrates how C++20 comparison works in real code, using `operator<=>` and ordering categories.

## 1. `Point`: defaulted `<=>` on floating-point members

```cpp
struct Point
{
  double x = 0.0;
  double y = 0.0;
  friend auto operator<=>(const Point &, const Point &) = default;
};
```

Key points:

- Comparison is lexicographic (`x`, then `y`).
- Because members are `double`, the category is `std::partial_ordering` (NaN can make comparisons unordered).
- Relational operators (`<`, `<=`, `>`, `>=`) and equality operators are available from the defaulted comparison machinery.

## 2. `Rational`: weak ordering + equivalence-based equality

```cpp
struct Rational
{
  int num = 0;
  int den = 1;
  friend std::weak_ordering operator<=>(Rational const &, Rational const &);
  friend bool operator==(Rational const &, Rational const &);
};
```

`<=>` compares values by cross multiplication and returns `less`, `greater`, or `equivalent`.

`==` is explicitly defined as:

```cpp
return std::is_eq(r1 <=> r2);
```

So `1/2` and `2/4` are considered equal here.

## 3. `Rational2`: weak ordering + representation equality

```cpp
struct Rational2
{
  int num = 0;
  int den = 1;
  friend std::weak_ordering operator<=>(Rational2 const &, Rational2 const &);
  friend bool operator==(Rational2 const &, Rational2 const &) = default;
};
```

- Ordering is still value-based (`1/2` equivalent to `2/4`).
- Equality is member-wise (`num` and `den` must match exactly).
- Therefore, `1/2` and `2/4` are equivalent by ordering but not `==`.

This is a useful example of keeping **ordering equivalence** and **object equality** distinct.

## 4. `Poset`: partial ordering with `unordered`

`Poset` compares vectors only if sizes are equal; otherwise it returns `std::partial_ordering::unordered`.

Practical checks:

- `std::is_eq(a <=> b)` for equivalence
- `(a <=> b) == std::partial_ordering::unordered` for incomparability

## 5. C++20 rules demonstrated

- Defaulted `<=>` chooses the weakest comparison category among members.
- Manual `<=>` does not automatically imply a manual `==`; if needed, define it explicitly.
- For partial ordering, relational expressions like `a < b` are `false` when values are unordered.

## 6. Expected runtime behavior

- `Point(4,5) < Point(5,6)` is `true`.
- `Rational{1,2}` and `Rational{2,4}` are equivalent and `== true`.
- `Rational2{1,2}` and `Rational2{2,4}` are equivalent but `== false`.
- Two `Poset` values with different vector sizes are unordered.
