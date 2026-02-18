# Ordering relations and the spaceship operator (`<=>`) in C++20

C++20 introduced **three-way comparison** with `operator<=>` and a clearer model for ordering and equivalence.

## What this example shows

`main.cpp` contains four small types:

- `Point`: defaulted `<=>` on two `double`s.
- `Rational`: manual `<=>` returning `std::weak_ordering`, and `==` defined from equivalence (`std::is_eq`).
- `Rational2`: same weak ordering as `Rational`, but defaulted `==` (representation equality).
- `Poset`: partial ordering with possible `unordered` results.

## C++20 comparison model (practical view)

- `std::strong_ordering`: total order, and equivalent values are substitutable for comparisons.
- `std::weak_ordering`: total order of equivalence classes; two objects may be equivalent without being `==`.
- `std::partial_ordering`: some pairs are unordered (not comparable).

### Important notes

- Defaulted `<=>` compares members lexicographically (declaration order).
- The return category is the weakest category among compared members.
- For floating-point members (`double`), defaulted `<=>` yields `std::partial_ordering` (because of NaN).
- If you **default** `<=>`, the compiler can also synthesize `==`.
- If you **write** `<=>` manually, `==` is not synthesized automatically.

## Comparison vs equivalence

Given `cmp = (a <=> b)`:

- `a < b` is based on `cmp < 0`.
- `a > b` is based on `cmp > 0`.
- `std::is_eq(cmp)` checks equivalence according to the chosen ordering.
- With `std::partial_ordering`, compare against `std::partial_ordering::unordered` to detect incomparable values.

`Rational` and `Rational2` are intentionally different:

- In `Rational`, `1/2` and `2/4` are equivalent and also `==` (because `==` is defined via `std::is_eq`).
- In `Rational2`, they are equivalent by ordering, but `==` is false (different stored numerator/denominator).

## Build and run

```bash
cd Examples/src/OrderingAndSpaceShip
make
./main
```

## Reference

- <https://en.cppreference.com/w/cpp/language/default_comparisons>
