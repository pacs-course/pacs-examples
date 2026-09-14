#include "operators.hpp"
#include "vectorExpr.hpp"

#include <concepts>
#include <utility>

// Ordinary arithmetic hides Expr's protected forwarding overloads. Explicit
// qualification in these test adapters forces those templates to instantiate.
template <class Concrete>
struct ForwardingProbe : Concrete
{
  using Concrete::Concrete;

  std::size_t forwarded_size() const { return this->ET23::Expr::size(); }

  decltype(auto) forwarded_at(std::size_t i) &
  {
    return this->ET23::Expr::operator[](i);
  }

  decltype(auto) forwarded_at(std::size_t i) const &
  {
    return this->ET23::Expr::operator[](i);
  }

  decltype(auto) forwarded_at(std::size_t i) &&
  {
    return std::move(*this).ET23::Expr::operator[](i);
  }
};

using VectorProbe = ForwardingProbe<ET23::Vector>;
using ValueProbe = ForwardingProbe<ET23::MinusExpr<ET23::Vector>>;

// Different ref-qualified implementations detect whether the forwarding
// wrapper preserves an rvalue receiver instead of treating self as an lvalue.
struct RefQualified : ET23::Expr
{
  double value = 4.0;
  std::size_t size() const { return 1; }
  double &operator[](std::size_t) & { return value; }
  double operator[](std::size_t) const & { return value; }
  double operator[](std::size_t) && { return 9.0; }
};

using QualifiedProbe = ForwardingProbe<RefQualified>;

static_assert(std::same_as<decltype(std::declval<VectorProbe &>().forwarded_at(0)),
                           double &>);
static_assert(std::same_as<decltype(std::declval<VectorProbe const &>().forwarded_at(0)),
                           double>);
static_assert(std::same_as<decltype(std::declval<ValueProbe &>().forwarded_at(0)),
                           double>);
static_assert(std::same_as<decltype(std::declval<ValueProbe const &>().forwarded_at(0)),
                           double>);
static_assert(std::same_as<decltype(std::declval<ValueProbe &&>().forwarded_at(0)),
                           double>);

int main()
{
  VectorProbe v(3, 2.0);
  v.forwarded_at(1) = 7.0;
  ValueProbe e(v);
  auto const &cv = v;
  auto const &ce = e;
  QualifiedProbe qualified;
  bool const correct = v.forwarded_size() == 3 && e.forwarded_size() == 3 &&
                       v[1] == 7.0 && cv.forwarded_at(1) == 7.0 &&
                       e.forwarded_at(1) == -7.0 && ce.forwarded_at(1) == -7.0 &&
                       std::move(e).forwarded_at(1) == -7.0 &&
                       qualified.forwarded_at(0) == 4.0 &&
                       std::move(qualified).forwarded_at(0) == 9.0;
  return correct ? 0 : 1;
}
