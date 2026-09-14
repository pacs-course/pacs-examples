#ifndef HH_Operators_HH23
#define HH_Operators_HH23
#include "expressionWrapper23.hpp"
#include <cassert>
#include <cmath>
namespace ET23
{
// Expression nodes contain operands and describe work; operator[] evaluates
// one element on demand. Concrete size()/operator[] members hide Expr's
// illustrative forwarding members. No virtual dispatch or CRTP is involved.

//! Lazy binary operation on two indexable, equally sized operands.
/*!
  Non-scalar operands are stored by const reference and must outlive evaluation.
  This also applies to intermediate expression nodes: materialize a nested
  formula in the same full expression, or give its intermediate nodes names.
  For example, Vector v = a + 2.0 * b is safe while its operands are alive;
  auto e = a + 2.0 * b leaves e referring to a destroyed multiplication node
  after the semicolon. The reference members do not extend operand lifetimes.
 */
template <class LO, class RO, class OP>
class BinaryOperator : public Expr
{
public:
  BinaryOperator(LO const &l, RO const &r) : M_lo(l), M_ro(r){};
  //! Evaluate element i; the caller must supply a valid index.
  double
  operator[](std::size_t i) const
  {
    return OP()(M_lo[i], M_ro[i]);
  }
  std::size_t
  size() const
  {
    // Equal sizes are a precondition; this diagnostic disappears with NDEBUG.
    // Direct operator[] calls do not perform this check.
    assert(M_lo.size() == M_ro.size());
    return M_lo.size();
  }

private:
  LO const &M_lo;
  RO const &M_ro;
};

//! Lazy unary operation; the referenced operand must outlive evaluation.
template <class RO, class OP>
class UnaryOperator : public Expr
{
public:
  UnaryOperator(RO const &r) : M_ro(r){};
  //! Evaluate element i; the caller must supply a valid index.
  double
  operator[](std::size_t i) const
  {
    return OP()(M_ro[i]);
  }
  std::size_t
  size() const
  {
    return M_ro.size();
  }

private:
  RO const &M_ro;
};

//! Lazy operation with a left double scalar, copied into the node.
// Only exactly double selects this specialization; use 2.0 rather than 2.
// The right operand remains a reference and supplies the expression size.
template <class RO, class OP>
class BinaryOperator<double, RO, OP>
  : public Expr
{
public:
  using LO = double;
  BinaryOperator(LO const &l, RO const &r) : M_lo(l), M_ro(r){};
  double
  operator[](std::size_t i) const
  {
    return OP()(M_lo, M_ro[i]);
  }
  std::size_t
  size() const
  {
    return M_ro.size();
  }

private:
  LO const  M_lo;
  RO const &M_ro;
};

//! Lazy operation with a right double scalar, copied into the node.
// The left operand remains a reference and supplies the expression size.
// Keeping the operand order is essential for noncommutative operations.
template <class LO, class OP>
class BinaryOperator<LO, double, OP>
  : public Expr
{
public:
  using RO = double;
  BinaryOperator(LO const &l, RO const &r) : M_lo(l), M_ro(r){};
  double
  operator[](std::size_t i) const
  {
    return OP()(M_lo[i], M_ro);
  }
  std::size_t
  size() const
  {
    return M_lo.size();
  }

private:
  LO const &M_lo;
  RO const  M_ro;
};

// Stateless functors implement the arithmetic on individual double values.
// These use ordinary const call operators. C++23 also permits static call
// operators, but that feature is independent of the explicit object parameters
// demonstrated by Expr.

//! Scalar addition (std::plus<double> from <functional> is an alternative).
struct Add
{
  double
  operator()(double i, double j) const
  {
    return i + j;
  }
};
//! Scalar multiplication; vector multiplication applies it elementwise.
struct Multiply
{
  double
  operator()(double i, double j) const
  {
    return i * j;
  }
};

//! Scalar subtraction, preserving left/right operand order.
struct Subtract
{
  double
  operator()(double i, double j) const
  {
    return i - j;
  }
};

//! Scalar negation.
struct Minus
{
  double
  operator()(double j) const
  {
    return -j;
  }
};

//! Scalar exponential; overflow and nonfinite inputs follow std::exp.
struct ExpOP
{
  double
  operator()(double j) const
  {
    return std::exp(j);
  }
};

//! Scalar natural logarithm; domain and range behavior follow std::log.
struct LogOP
{
  double
  operator()(double j) const
  {
    return std::log(j);
  }
};

// Alias templates bind each scalar functor to its expression node type.
template <class LO, class RO> using AddExpr = BinaryOperator<LO, RO, Add>;

template <class LO, class RO> using MultExpr = BinaryOperator<LO, RO, Multiply>;

template <class LO, class RO> using SubExpr = BinaryOperator<LO, RO, Subtract>;

template <class RO> using MinusExpr = UnaryOperator<RO, Minus>;

template <class RO> using ExpExpr = UnaryOperator<RO, ExpOP>;

template <class RO> using LogExpr = UnaryOperator<RO, LogOP>;

// Public expression-building functions: no element evaluation happens here.
// These templates are unconstrained. Supported operands provide size() and
// const indexing, or are a double paired with such an operand. Prefer ADL
// through an ET23 operand to importing these operators for unrelated types.
// std::vector<double> operands are referenced directly, without conversion to
// Vector. Scalar/scalar arithmetic should use the built-in operators.

//! Build an elementwise addition expression.
template <class LO, class RO>
inline AddExpr<LO, RO>
operator+(LO const &l, RO const &r)
{
  return AddExpr<LO, RO>(l, r);
}

//! Build an elementwise multiplication expression (not a dot product).
template <class LO, class RO>
inline MultExpr<LO, RO>
operator*(LO const &l, RO const &r)
{
  return MultExpr<LO, RO>(l, r);
}

//! Build an elementwise subtraction expression.
template <class LO, class RO>
inline SubExpr<LO, RO>
operator-(LO const &l, RO const &r)
{
  return SubExpr<LO, RO>(l, r);
}

//! Build an elementwise negation expression.
template <class RO>
inline MinusExpr<RO>
operator-(RO const &r)
{
  return MinusExpr<RO>(r);
}

//! Build an elementwise exponential expression.
template <class RO>
inline ExpExpr<RO>
exp(RO const &r)
{
  return ExpExpr<RO>(r);
}

//! Build an elementwise natural-logarithm expression.
template <class RO>
inline LogExpr<RO>
log(RO const &r)
{
  return LogExpr<RO>(r);
}
} // namespace ET23
#endif
