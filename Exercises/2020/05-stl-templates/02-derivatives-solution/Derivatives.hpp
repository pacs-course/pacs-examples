#ifndef DERIVATIVES_HPP
#define DERIVATIVES_HPP

#include <functional>
#include <type_traits>

namespace DifferenceType
{
  struct BACKWARD; // forward declaration

  struct FORWARD
  {
    using otherType = BACKWARD;
  };

  struct BACKWARD
  {
    using otherType = FORWARD;
  };
} // namespace DifferenceType

//! Computes the Nth derivative of a function by finite differences.
/*!
  \tparam N The order of the derivative.
  \tparam F The callable object of signature T (T const &).
  \tparam T The argument and return type of the callable object.
  \tparam DT The type of differencing: either
  DifferenceType::FORWARD or DifferenceType::BACKWARD.
 */
template <unsigned N,
          typename F,
          typename T  = double,
          typename DT = DifferenceType::FORWARD>
class NthDerivative
{
public:
  //! The previous derivative is approximated using
  //! the other type to get a more stable algorithm.
  using PreviousDerivative =
    NthDerivative<N - 1, F, T, typename DT::otherType>;

  //! Constructor.
  /*!
    \param f The function (callable object).
    \param h The spacing to be used in the formula.
   */
  NthDerivative(const F &f, const T &h)
    : pDerivative{f, h}
    , h{h}
  {}

  //! The call operator that computes the derivative.
  /*!
    \param x The point where the derivative is computed.
    \return The derivative value.
   */
  T
  operator()(const T &x) const
  {
    // Since C++17.
    if constexpr (std::is_same<DifferenceType::FORWARD, DT>::value)
      return (pDerivative(x + h) - pDerivative(x)) / h;
    else
      return (pDerivative(x) - pDerivative(x - h)) / h;
  }

private:
  PreviousDerivative pDerivative;
  T                  h;
};

//! Specialization for first derivative.
template <typename F, typename T, typename DT>
class NthDerivative<1u, F, T, DT>
{
public:
  //! Constructor.
  NthDerivative(const F &f, const T &h)
    : pDerivative{f}
    , h{h}
  {}

  T
  operator()(const T &x) const
  {
    if constexpr (std::is_same<DifferenceType::FORWARD, DT>::value)
      return (pDerivative(x + h) - pDerivative(x)) / h;
    else
      return (pDerivative(x) - pDerivative(x - h)) / h;
  }

private:
  F pDerivative;
  T h;
};

//! Only for consistency: 0th derivative.
//!
//! It prevents infinite loops if someone creates
//! a 0-th derivative object.
template <typename F, typename T, typename DT>
class NthDerivative<0u, F, T, DT>
{
public:
  //! Constructor.
  NthDerivative(const F &f, const T &h)
    : pDerivative{f}
    , h{h}
  {}

  //! The call operator returns the value of the function itself.
  T
  operator()(const T &x) const
  {
    return pDerivative(x);
  }

private:
  F pDerivative;
  T h;
};

//! Utility to simplify the creation of a NthDerivative object.
/*
 * Example of usage:
 * /code
 * auto f = [](const double & x) { return x*std::sin(x); };
 * double h = 0.1;
 * auto d = make_forward_derivative<3>(f, h);
 * auto value = d(3.0); // 3rd derivative at 3.0.
 * /endcode
 *
 * /param f A callable function with the rigth signature.
 * /param h The step for computing derivatives.
 */
template <unsigned N, typename F, typename T>
auto
make_forward_derivative(const F &f, const T &h)
{
  return NthDerivative<N, F, T, DifferenceType::FORWARD>{f, h};
}

//! Utility to simplify the creation of a NthDerivative object.
/*
 * Example of usage:
 * /code
 * auto f = [](const double & x) { return x*std::sin(x); };
 * double h = 0.1;
 * auto d = make_backward_derivative<3>(f, h);
 * /endcode
 * /param f A callable function with the rigth signature.
 * /param h The step for computing derivatives.
 */
template <unsigned N, typename F, typename T>
auto
make_backward_derivative(const F &f, const T &h)
{
  return NthDerivative<N, F, T, DifferenceType::BACKWARD>{f, h};
}

#endif
