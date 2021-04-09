#ifndef DERIVATIVES_HPP
#define DERIVATIVES_HPP

#include <functional>
#include <type_traits>

namespace Differentiation
{
  class BACKWARD; // Forward declaration.

  class FORWARD
  {
  public:
    using OTHER_TYPE = BACKWARD;
  };

  class BACKWARD
  {
  public:
    using OTHER_TYPE = FORWARD;
  };
} // namespace Differentiation

/**
 * Computes the Nth derivative of a function by finite differences.
 *
 * @tparam N The order of the derivative.
 * @tparam FunctionType The callable object of signature T (T const &).
 * @tparam NumberType The argument and return type of the callable object.
 * @tparam DifferenceType The type of differencing: either
 * Differentiation::FORWARD or Differentiation::BACKWARD.
 */
template <unsigned int N,
          class FunctionType,
          class NumberType     = double,
          class DifferenceType = Differentiation::FORWARD>
class NthDerivative
{
public:
  /**
   * Constructor.
   *
   * @param f The function (callable object).
   * @param h The spacing to be used in the formula.
   */
  NthDerivative(const FunctionType &f, const NumberType &h)
    : previous_derivative{f, h}
    , h{h}
  {}

  /**
   * The call operator that computes the derivative.
   *
   * @param x The point where the derivative is computed.
   * @return The derivative value.
   */
  NumberType
  operator()(const NumberType &x) const
  {
    // Since C++17.
    if constexpr (std::is_same_v<Differentiation::FORWARD, DifferenceType>)
      return (previous_derivative(x + h) - previous_derivative(x)) / h;
    else
      return (previous_derivative(x) - previous_derivative(x - h)) / h;
  }

private:
  /// The previous derivative is approximated using
  /// the other type to get a more stable algorithm.
  using PreviousDerivative = NthDerivative<N - 1,
                                           FunctionType,
                                           NumberType,
                                           class DifferenceType::OTHER_TYPE>;

  PreviousDerivative previous_derivative;
  NumberType         h;
};

/// Specialization for first derivative.
template <class FunctionType, class NumberType, class DifferenceType>
class NthDerivative<1u, FunctionType, NumberType, DifferenceType>
{
public:
  /// Constructor.
  NthDerivative(const FunctionType &f, const NumberType &h)
    : function{f}
    , h{h}
  {}

  NumberType
  operator()(const NumberType &x) const
  {
    if constexpr (std::is_same_v<Differentiation::FORWARD, DifferenceType>)
      return (function(x + h) - function(x)) / h;
    else
      return (function(x) - function(x - h)) / h;
  }

private:
  FunctionType function;
  NumberType   h;
};

/**
 * For consistency: 0th derivative.
 *
 * It prevents infinite loops if a user instantiates a 0-th derivative object.
 */
template <class FunctionType, class NumberType, class DifferenceType>
class NthDerivative<0u, FunctionType, NumberType, DifferenceType>
{
public:
  /// Constructor.
  NthDerivative(const FunctionType &f, const NumberType &h)
    : function{f}
    , h{h}
  {}

  /// The call operator returns the value of the function itself.
  NumberType
  operator()(const NumberType &x) const
  {
    return function(x);
  }

private:
  FunctionType function;
  NumberType   h;
};

/**
 * Utility to simplify the creation of a NthDerivative object.
 *
 * Example of usage:
 * @code
 * auto f = [](const double & x) { return x*std::sin(x); };
 * double h = 0.1;
 * auto d = make_forward_derivative<3>(f, h);
 * auto value = d(3.0); // 3rd derivative at 3.0.
 * @endcode
 *
 * @param f A callable function with the rigth signature.
 * @param h The step for computing derivatives.
 */
template <unsigned int N, class FunctionType, class NumberType>
auto
make_forward_derivative(const FunctionType &f, const NumberType &h)
{
  return NthDerivative<N, FunctionType, NumberType, Differentiation::FORWARD>{
    f, h};
}

/**
 * Utility to simplify the creation of a NthDerivative object.
 *
 * Example of usage:
 * @code
 * auto f = [](const double & x) { return x*std::sin(x); };
 * const double h = 0.1;
 * auto d = make_backward_derivative<3>(f, h);
 * @endcode
 * @param f A callable function with the rigth signature.
 * @param h The step for computing derivatives.
 */
template <unsigned int N, class FunctionType, class NumberType>
auto
make_backward_derivative(const FunctionType &f, const NumberType &h)
{
  return NthDerivative<N, FunctionType, NumberType, Differentiation::BACKWARD>{
    f, h};
}

#endif /* DERIVATIVES_HPP */
