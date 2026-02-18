/*
 * main.cpp
 *
 *  Created on: Jan 15, 2022
 *      Author: forma
 */
#include <algorithm>
#include <compare>
#include <iomanip>
#include <iostream>
#include <set>
#include <unordered_set>

#include <vector>
// C++20 introduces:
// 1) comparison categories: strong / weak / partial ordering
// 2) the three-way comparison operator "<=>"
// This example shows how those concepts map to real classes.
/*!
 * Point with defaulted three-way comparison.
 *
 * Teaching note:
 * - Defaulted <=> compares members in declaration order (x, then y).
 * - Since members are double, the category is partial_ordering
 *   (NaN can make comparisons unordered).
 */
struct Point
{
  double x = 0.0;
  double y = 0.0;
  //! Default <=> lets the compiler generate consistent comparisons.
  //! Using auto is convenient: the compiler deduces the proper category.
  friend auto operator<=>(const Point &, const Point &) = default;
};

/*!
 * Rational number represented as numerator/denominator, without normalization.
 *
 * Example: 1/2 and 2/4 store different data but represent the same value.
 *
 */
struct Rational
{
  int num = 0;
  int den = 1;
  /*! @brief Three-way comparison by numeric value.
   *
   * We return std::weak_ordering:
   * - every pair is comparable by value,
   * - but equivalent values may have different representations.
   */
  friend std::weak_ordering operator<=>(Rational const &r1, Rational const &r2);
  /*!
   * Equality chosen to mean "same mathematical value".
   * Here we define == consistently with the comparison equivalence class.
   * @param r1 A rational
   * @param r2 A rational
   * @return true when r1 and r2 are equivalent according to <=>.
   */
  friend bool operator==(Rational const &r1, Rational const &r2);
};

inline std::weak_ordering
operator<=>(Rational const &r1, Rational const &r2)
{
  using po = std::weak_ordering;
  // Compare r1.num/r1.den and r2.num/r2.den without converting to floating
  // point. Use long long intermediates to reduce overflow risk for typical int
  // inputs.
  auto a = 1LL * r1.num * r2.den;
  auto b = 1LL * r1.den * r2.num;
  if(a < b)
    return po::less;
  if(b < a)
    return po::greater;
  else
    return po::equivalent;
}

inline bool
operator==(Rational const &r1, Rational const &r2)
{
  // "Equal" here means "equivalent by value".
  // This is consistent with the equivalence
  // class defined by <=>, so we can use std::is_eq.
  return std::is_eq(r1 <=> r2);
  // I may want instead to check representation equality (num and den), but that
  // would be inconsistent with the ordering equivalence class:
  //
  // return r1.num == r2.num && r1.den == r2.den;
  //
  // This is what the compiler would generate if I defaulted ==.
  //
}
/*!
 * Same representation as Rational, but a different design choice for ==.
 *
 */
struct Rational2
{
  int num = 0;
  int den = 1;
  /*! @brief Value-based ordering, same as Rational.
   *
   * 1/2 and 2/4 are equivalent in ordering.
   */
  friend auto operator<=>(Rational2 const &r1, Rational2 const &r2);
  /*!
   * Here == is defaulted: it checks stored representation (num and den).
   * So 1/2 != 2/4 for ==, even if they are equivalent by <=>.
   * @param r1 A rational
   * @param r2 A rational
   * @return true if both num and den are equal.
   */
  friend bool operator==(Rational2 const &r1, Rational2 const &r2) = default;
};

inline auto
operator<=>(Rational2 const &r1, Rational2 const &r2)
{
  using po = std::weak_ordering;
  auto a = 1LL * r1.num * r2.den;
  auto b = 1LL * r1.den * r2.num;
  if(a < b)
    return po::less;
  if(b < a)
    return po::greater;
  else
    return po::equivalent;
}

struct Poset
{
  /*!
   * @brief Three-way comparison with explicit incomparability.
   *
   * Rule for this toy example:
   * - if vectors are equal: equivalent
   * - if sizes differ: unordered (not comparable)
   * - otherwise: lexicographic less/greater
   *
   * Because we can return "unordered", the correct category is
   * partial_ordering.
   */
  std::vector<int> data;
  std::partial_ordering
  operator<=>(Poset const &r2) const
  {
    using po = std::partial_ordering;
    if(r2.data.size() !=
       data.size()) // different sizes are unordered (not comparable)
      return po::unordered;
    if(std::ranges::lexicographical_compare(
         data, r2.data)) // lexicographic order for vectors of the same size
      return po::less;
    if(std::ranges::equal(data, r2.data)) // equal vectors are equivalent
      return po::equivalent;

    return po::greater;
  }
};

int
main()
{
  // Case 1: defaulted <=> on Point
  Point a{4., 5.};
  Point b{5., 6.};
  std::cout << "Two different points a=(4,5) and b=(5,6)\n";
  std::cout << std::boolalpha << "(a < b)? " << (a < b) << ", (a == b)? "
            << (a == b) << std::endl;

  {
    // Case 2: == chosen as value equivalence
    Rational r1{1, 2};
    Rational r2{2, 4};
    std::cout << "Two Rationals r1=1/2 and r2=2/4\n";
    std::cout << std::boolalpha << "r1 eqv r2? " << std::is_eq(r1 <=> r2)
              << ", r1==r2? " << (r1 == r2) << std::endl;
    std::cout << std::boolalpha << "r1 < r2? " << (r1 < r2) << std::endl;
  }
  {
    // Case 3: == chosen as representation equality
    Rational2 r1{1, 2};
    Rational2 r2{2, 4};
    std::cout << "Two Rational2s r1=1/2 and r2=2/4\n";
    std::cout << std::boolalpha << "r1 eqv r2? " << std::is_eq(r1 <=> r2)
              << ", r1==r2? " << (r1 == r2) << std::endl;
    std::cout << std::boolalpha << "r1 < r2? " << (r1 < r2) << std::endl;
  }

  // Case 4: partial ordering with an unordered outcome
  Poset ap{{1, 2, 3, 4}};
  Poset bp{{1, 2}};
  std::cout << "These two posets, ap and bp, are not comparable.\n";
  auto pcmp = (ap <=> bp);
  std::cout << std::boolalpha << "ap<bp? " << (ap < bp) << ", ap>=bp? "
            << (ap >= bp) << ", ap eq bp? " << std::is_eq(pcmp)
            << ", unordered? " << (pcmp == std::partial_ordering::unordered)
            << std::endl;
}
