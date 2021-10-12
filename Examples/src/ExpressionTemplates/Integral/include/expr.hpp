#ifndef _EXPR_HPP_
#define _EXPR_HPP_

#include "operation.hpp"

namespace ET
{
/*!
 * @struct BinaryExpr
 * this struct models an expression that applies
 * an operation on 2 objects
 */
template <typename expr1T, typename expr2T, typename opT> struct BinaryExpr
{
  BinaryExpr(expr1T e1, expr2T e2, opT op = opT())
    : M_e1(e1), M_e2(e2), M_op(op)
  {}

  double
  eval(double pt) const
  {
    return M_op(M_e1.eval(pt), M_e2.eval(pt));
  }

private:
  typename Var::exprTraits<expr1T>::expr_Type M_e1;
  typename Var::exprTraits<expr2T>::expr_Type M_e2;
  opT                                         M_op;
};

/*!
 * @struct UnaryExpr
 * this struct models an expression that applies
 * an operation on a single object
 */
template <typename exprT, typename opT> struct UnaryExpr
{
  UnaryExpr(exprT e, opT op = opT()) : M_e(e), M_op(op) {}

  double
  eval(double pt) const
  {
    return M_op(M_e.eval(pt));
  }

private:
  typename Var::exprTraits<exprT>::expr_Type M_e;
  opT                                        M_op;
};

/// @brief operator overload of operator+
template <typename expr1T, typename expr2T>
BinaryExpr<expr1T, expr2T, Operation::Plus<double> >
operator+(expr1T e1, expr2T e2)
{
  return BinaryExpr<expr1T, expr2T, Operation::Plus<double> >(e1, e2);
}

/// @brief operator overload of operator-
template <typename expr1T, typename expr2T>
BinaryExpr<expr1T, expr2T, Operation::Minus<double> >
operator-(expr1T e1, expr2T e2)
{
  return BinaryExpr<expr1T, expr2T, Operation::Minus<double> >(e1, e2);
}

/// @brief operator overload of operator*
template <typename expr1T, typename expr2T>
BinaryExpr<expr1T, expr2T, Operation::Multiply<double> >
operator*(expr1T e1, expr2T e2)
{
  return BinaryExpr<expr1T, expr2T, Operation::Multiply<double> >(e1, e2);
}

/// @brief operator overload of operator/
template <typename expr1T, typename expr2T>
BinaryExpr<expr1T, expr2T, Operation::Divide<double> >
operator/(expr1T e1, expr2T e2)
{
  return BinaryExpr<expr1T, expr2T, Operation::Divide<double> >(e1, e2);
}

/// @brief exponential function
template <typename exprT>
UnaryExpr<exprT, Operation::Exp<double> >
exp(exprT e)
{
  return UnaryExpr<exprT, Operation::Exp<double> >(e);
}

/// @brief evaluation function
template <typename exprT>
double
eval(exprT e, double where)
{
  return e.eval(where);
}

} // namespace ET

#endif // _EXPR_HPP_
