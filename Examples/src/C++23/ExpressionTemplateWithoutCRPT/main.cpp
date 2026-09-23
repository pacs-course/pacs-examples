#include "operators.hpp"
#include "vectorExpr.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

// Build with a compiler supporting C++23 explicit object parameters, e.g.:
// clang++-20 -std=c++23 -Wall -Wextra -Wpedantic main.cpp -o main
// ./main
//
// This is an executable tutorial: every numerical example is checked, and a
// failed check produces a nonzero exit status (also when NDEBUG is defined).
namespace
{
class Tests
{
  unsigned int checks = 0;
  unsigned int failures = 0;

public:
  void
  check(std::string_view label, bool condition)
  {
    ++checks;
    std::cout << (condition ? "  PASS: " : "  FAIL: ") << label << '\n';
    if(!condition)
      ++failures;
  }

  // Keep the concrete expression type: converting to Expr const& here would
  // discard the static type required for expression-template dispatch.
  template <class Expression>
  void
  values(std::string_view label, Expression const &expression,
         std::vector<double> const &expected)
  {
    bool const same_size = expression.size() == expected.size();
    bool       equal = same_size;
    if(same_size)
      for(std::size_t i = 0; i < expected.size(); ++i)
        {
          double const actual = expression[i];
          double const scale = std::max(1.0, std::abs(expected[i]));
          if(!std::isfinite(actual) ||
             std::abs(actual - expected[i]) > 1.e-12 * scale)
            {
              equal = false;
              std::cout << "    index " << i << ": expected " << expected[i]
                        << ", got " << actual << '\n';
            }
        }
    else
      std::cout << "    expected size " << expected.size() << ", got "
                << expression.size() << '\n';

    check(label, equal);
  }

  int
  result() const
  {
    std::cout << '\n' << checks - failures << '/' << checks
              << " checks passed; " << failures << " failed.\n";
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
  }
};
} // namespace

int
main()
{
  using ET23::Vector;
  Tests tests;

  std::cout << "1. Vector owns its storage\n";
  Vector empty;
  tests.values("default construction", empty, {});
  Vector zeros(3);
  tests.values("size construction initializes to zero", zeros, {0, 0, 0});
  Vector filled(3, 2.5);
  tests.values("size and value construction", filled, {2.5, 2.5, 2.5});

  std::vector<double> source{1, 2, 4};
  Vector a(source); // Explicit construction copies an lvalue std::vector.
  source[0] = 99;
  tests.values("construction owns a copy of std::vector", a, {1, 2, 4});
  Vector b(std::vector<double>{3, 5, 2}); // An rvalue can transfer its storage.
  tests.values("construction from an rvalue std::vector", b, {3, 5, 2});

  Vector copied(a);
  copied[0] = 10;
  tests.values("mutable indexing", copied, {10, 2, 4});
  tests.values("Vector copies have independent storage", a, {1, 2, 4});
  Vector moved(std::move(copied));
  tests.values("Vector move construction", moved, {10, 2, 4});
  // A moved-from vector is valid, but its contents are unspecified.
  copied = a;
  tests.values("Vector copy assignment", copied, {1, 2, 4});
  moved = std::move(copied);
  tests.values("Vector move assignment", moved, {1, 2, 4});
  Vector const &constant_a = a;
  tests.check("const indexing", constant_a[2] == 4.0);

  std::cout << "\n2. Access to std::vector storage and iterators\n";
  Vector storage(std::vector<double>{1, 2});
  std::vector<double> &view = storage; // Conversion returns a reference.
  view.emplace_back(3);
  storage.as_vector().emplace_back(4); // Named alternative to the conversion.
  for(double &value : storage) // ADL finds ET23::begin and ET23::end.
    value *= 2;
  tests.values("conversion, as_vector, and mutable range-for", storage,
               {2, 4, 6, 8});
  tests.check("mutable accessors refer to the same storage",
              &view == &storage.as_vector());
  Vector const &constant_storage = storage;
  std::vector<double> const &constant_view = constant_storage;
  tests.check("const accessors refer to the same storage",
              &constant_view == &constant_storage.as_vector());
  // The headers supply cbegin/cend for const Vector, but no const begin/end.
  // Use these explicitly instead of range-for over a const Vector.
  double total = 0;
  for(auto it = ET23::cbegin(constant_storage);
      it != ET23::cend(constant_storage); ++it)
    total += *it;
  tests.check("const iteration through cbegin/cend", total == 20.0);

  std::cout << "\n3. Elementwise arithmetic builds lazy expression objects\n"
               "   Inputs: a = {1, 2, 4}, b = {3, 5, 2}.\n"
               "   Multiplication is elementwise, not a dot product.\n";
  auto sum = a + b;
  static_assert(std::is_base_of_v<ET23::Expr, decltype(sum)>);
  static_assert(!std::is_same_v<Vector, decltype(sum)>);
  tests.values("a + b", sum, {4, 7, 6});
  tests.values("a - b", a - b, {-2, -3, 2});
  tests.values("a * b", a * b, {3, 10, 8});
  tests.values("-a", -a, {-1, -2, -4});

  // Scalar specializations are for exactly double: write 2.0, not 2.
  // Scalars are stored by value; vector/expression operands by const reference.
  tests.values("a + 2.0", a + 2.0, {3, 4, 6});
  tests.values("2.0 + a", 2.0 + a, {3, 4, 6});
  tests.values("a - 2.0", a - 2.0, {-1, 0, 2});
  tests.values("2.0 - a (operand order matters)", 2.0 - a, {1, 0, -2});
  tests.values("a * 2.0", a * 2.0, {2, 4, 8});
  tests.values("2.0 * a", 2.0 * a, {2, 4, 8});

  std::cout << "\n4. Unary functions and nested expressions\n";
  std::vector<double> expected_exp;
  std::vector<double> expected_log;
  std::vector<double> expected_formula;
  for(std::size_t i = 0; i < a.size(); ++i)
    {
      expected_exp.push_back(std::exp(a[i]));
      expected_log.push_back(std::log(a[i]));
      expected_formula.push_back(std::log(a[i] + 2.0 * b[i]) -
                                 std::exp(-a[i]) + a[i] * b[i]);
    }
  tests.values("exp(a)", ET23::exp(a), expected_exp);
  tests.values("log(a), with positive inputs", ET23::log(a), expected_log);
  tests.values("log(exp(a))", ET23::log(ET23::exp(a)), {1, 2, 4});
  tests.values("a + b + a (nested binary nodes)", a + b + a, {5, 9, 10});
  tests.values("log(a + 2.0*b) - exp(-a) + a*b",
               ET23::log(a + 2.0 * b) - ET23::exp(-a) + a * b,
               expected_formula);

  // Temporaries in the preceding calls live until the end of each full
  // expression, so evaluation inside values() is safe. In contrast:
  //   auto dangling = a + b + a;
  // would retain a reference to the destroyed temporary node (a + b).
  // Name every intermediate node when retaining a composite expression.
  auto scaled_b = 2.0 * b;
  auto retained = a + scaled_b;
  tests.values("retained expression with named intermediate", retained,
               {7, 12, 8});

  std::cout << "\n5. Lazy evaluation and operand lifetimes\n";
  Vector changing(std::vector<double>{1, 2, 4});
  auto lazy_sum = changing + b;
  tests.values("lazy expression before mutation", lazy_sum, {4, 7, 6});
  changing[0] = 10;
  tests.values("same expression observes operand mutation", lazy_sum,
               {13, 7, 6});
  double factor = 2.0;
  auto scaled = factor * changing;
  auto shifted = changing + factor;
  factor = 100.0;
  tests.values("left scalar captured by value", scaled, {20, 4, 8});
  tests.values("right scalar captured by value", shifted, {12, 4, 6});
  // Referenced vectors and intermediate nodes must outlive evaluation.
  // Binary vector operands must have equal sizes: size() asserts this in
  // debug builds. Mismatched operands are deliberately not evaluated here;
  // with NDEBUG they can cause out-of-bounds access rather than an exception.
  // Division and general scalar types are not supplied by these headers.

  std::cout << "\n6. Empty expressions\n";
  Vector another_empty;
  tests.values("empty + empty", empty + another_empty, {});
  tests.values("scalar times empty", 2.0 * empty, {});
  tests.values("exp(empty)", ET23::exp(empty), {});

  std::cout << "\n7. Materialization: expressions should produce owning Vectors\n"
               "   Vector's constrained constructor and assignment templates\n"
               "   preserve the concrete expression type during evaluation.\n"
               "   Taking const Expr& instead would call the nonvirtual base\n"
               "   fallbacks: explicit object parameters do not provide\n"
               "   dynamic dispatch through a base reference.\n";
  Vector constructed = a + b;
  tests.values("construct Vector from a + b", constructed, {4, 7, 6});
  Vector assigned(1, -99.0);
  assigned = a + b; // Assignment should resize the destination to three.
  tests.values("assign expression and resize destination", assigned, {4, 7, 6});
  Vector composite = ET23::log(a + 2.0 * b) - ET23::exp(-a) + a * b;
  tests.values("materialize a complete formula in one statement", composite,
               expected_formula);

  Vector snapshot = lazy_sum;
  changing[0] = 20;
  tests.values("materialization should preserve a snapshot", snapshot,
               {13, 7, 6});
  tests.values("original lazy expression still tracks its operands", lazy_sum,
               {23, 7, 6});

  // Same-size, pointwise self-assignment can safely read and then overwrite
  // each element. This does not imply arbitrary aliasing/resizing is safe.
  Vector in_place(a);
  in_place = -in_place;
  tests.values("pointwise self-assignment: v = -v", in_place, {-1, -2, -4});
  Vector accumulated(a);
  accumulated = accumulated + b;
  tests.values("pointwise self-assignment: v = v + b", accumulated, {4, 7, 6});

  // std::vector operands work because they provide size() and operator[].
  // The expression refers directly to the container: no Vector conversion
  // occurs. Keep an ET23 operand so ADL can find the arithmetic operators.
  std::vector<double> raw{10, 20, 30};
  tests.values("Vector + std::vector expression", a + raw, {11, 22, 34});
  tests.values("std::vector + Vector expression", raw + a, {11, 22, 34});
  Vector mixed = a + raw;
  tests.values("materialize mixed container expression", mixed, {11, 22, 34});
  Vector empty_result = empty + another_empty;
  tests.values("materialize an empty expression", empty_result, {});
  assigned = empty + another_empty;
  tests.values("assign an empty expression", assigned, {});

  return tests.result();
}
