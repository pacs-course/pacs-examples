#ifndef HH_Operators_HHa
#define HH_Operators_HHa
#include "expressionWrapper.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include <utility>
// Operators can be defined throus static or non static call operators.
namespace ET
{
//  CLASSES THAT ENCAPSULATE OPERATIONS
//! Binary operator expression.
template <class LO, class RO, class OP>
class BinaryOperator : public Expr<BinaryOperator<LO, RO, OP> >
{
public:
  BinaryOperator(LO const &l, RO const &r) : M_lo(l), M_ro(r){};
  // Applies operation on operands
  double
  operator[](std::size_t i) const
  {
    return OP()(M_lo[i], M_ro[i]);
  }
  std::size_t
  size() const
  {
    // disabled when NDEBUG is set. Checks if both operands have the same size
    assert(M_lo.size() == M_ro.size());
    return M_lo.size();
  }

private:
  LO const &M_lo;
  RO const &M_ro;
};

//! Unary operator expression.
template <class RO, class OP>
class UnaryOperator : public Expr<UnaryOperator<RO, OP> >
{
public:
  UnaryOperator(RO const &r) : M_ro(r){};
  // Applies operation on operands
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

//! Specialization for operation by a scalar
template <class RO, class OP>
class BinaryOperator<double, RO, OP>
  : public Expr<BinaryOperator<double, RO, OP> >
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

//! Specialization for operation by a scalar
template <class LO, class OP>
class BinaryOperator<LO, double, OP>
  : public Expr<BinaryOperator<LO, double, OP> >
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

//  THE BASIC OPERATIONS AT ELEMENT LEVEL
//! The basic Addition
/*!
  Note that we can use directly the functors
  provided by the standard library!
  /code
  using Add = std::add<double>;
  /endcode
*/
struct Add
{
  double
  operator()(double i, double j) const
  {
    return i + j;
  }
};
//! The basic Multiplication
struct Multiply
{
  double
  operator()(double i, double j) const
  {
    return i * j;
  }
};

//! The basic Subtraction
struct Subtract
{
  double
  operator()(double i, double j) const
  {
    return i - j;
  }
};

//! Minus operator
struct Minus
{
  double
  operator()(double j) const
  {
    return -j;
  }
};

// Some fancier operators
//! Exponential
struct ExpOP
{
  double
  operator()(double j) const
  {
    return std::exp(j);
  }
};

//! Logarithm
struct LogOP
{
  double
  operator()(double j) const
  {
    return std::log(j);
  }
};

// WRAPPING THE BASE OPERATIONS INTO THE OPERATION CLASSES: ARE JUST TYPEDEFS
template <class LO, class RO> using AddExpr = BinaryOperator<LO, RO, Add>;

template <class LO, class RO> using MultExpr = BinaryOperator<LO, RO, Multiply>;

template <class LO, class RO> using SubExpr = BinaryOperator<LO, RO, Subtract>;

template <class RO> using MinusExpr = UnaryOperator<RO, Minus>;

template <class RO> using ExpExpr = UnaryOperator<RO, ExpOP>;

template <class RO> using LogExpr = UnaryOperator<RO, LogOP>;

//  USER LEVEL OPERATORS: THESE ARE THE ONLY ONES THE USER WILL ADOPT

//! Addition of  expression
template <class LO, class RO>
inline AddExpr<LO, RO>
operator+(LO const &l, RO const &r)
{
  return AddExpr<LO, RO>(l, r);
}

//! Multiplication of expressions
template <class LO, class RO>
inline MultExpr<LO, RO>
operator*(LO const &l, RO const &r)
{
  return MultExpr<LO, RO>(l, r);
}

template <class LO, class RO>
inline SubExpr<LO, RO>
operator-(LO const &l, RO const &r)
{
  return SubExpr<LO, RO>(l, r);
}

template <class RO>
inline MinusExpr<RO>
operator-(RO const &r)
{
  return MinusExpr<RO>(r);
}

//! Exponential
template <class RO>
inline ExpExpr<RO>
exp(RO const &r)
{
  return ExpExpr<RO>(r);
}

//! Logarithm
template <class RO>
inline LogExpr<RO>
log(RO const &r)
{
  return LogExpr<RO>(r);
}
} // namespace ET
#endif
