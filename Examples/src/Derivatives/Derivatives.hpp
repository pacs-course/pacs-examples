/*!
 * @file Derivatives.hpp
 * @brief Finite difference derivative approximation library
 * 
 * This header provides a comprehensive set of tools for computing numerical derivatives
 * of callable objects using finite difference methods. It supports multiple difference
 * schemes (forward, backward, and centered) and arbitrary derivative orders.
 * 
 * @details
 * The library is built around the `NthDerivative` template class, which computes the
 * Nth derivative of a function using finite differences. Key features include:
 * 
 * - **Multiple Difference Schemes**: Forward, backward, and centered difference methods
 * - **Arbitrary Derivative Orders**: Recursive computation of Nth derivatives through
 *   composition of lower-order derivatives, with automatic alternation between difference schemes
 * - **Flexible Input Types**: Works with any callable object (functions, lambdas, functors)
 *   and any numeric type supporting the required arithmetic operations
 * - **Compile-time Optimization**: Uses C++20 concepts and `constexpr` for type safety
 *   and compile-time computations
 * 
 * @namespace apsc
 * Main namespace containing all derivative computation utilities.
 * 
 * @namespace apsc::DifferenceType
 * Namespace defining difference scheme types (FORWARD, BACKWARD, CENTERED) and
 * a concept for validating difference types.
 * 
 * @see NthDerivative
 * @see makeForwardDerivative
 * @see makeBackwardDerivative
 * @see makeCenteredDerivative
 * @see derive
 */
#ifndef HH__DERIVATIVESHPP_HH
#define HH__DERIVATIVESHPP_HH
#include <concepts>
#include <functional>
#include <type_traits>
#include <utility>
namespace apsc
{
  namespace DifferenceType
  {
    struct BACKWARD; // forward declaration
    /*!
     * It expresses the concept of a forward difference
     */
    struct FORWARD
    {
      //! I alternate among different types when performing higher derivatives
      using otherType = BACKWARD;
    };
    /*!
     * It expresses the concept of a backward difference
     */
    struct BACKWARD
    {
      using otherType = FORWARD;
    };

    struct CENTERED
    {
      using otherType = CENTERED;
    };

/*!
@brief Concept for valid difference types.
*/
    template <typename DT>
    concept Valid = std::same_as<DT, FORWARD> || std::same_as<DT, BACKWARD> ||
                    std::same_as<DT, CENTERED>;
  } // namespace DifferenceType
/*!
@brief Concept for a differentiable function.
A differentiable function is a callable object that takes a single argument of type T
and returns a value that can be converted to T.
@note Using F& and not just F is important to allow for stateful functors 
and lambdas that capture by reference. It means the callable is tested as 
a non-temporary object.
*/
  template <typename F, typename T>
  concept DifferentiableFunction =
    std::invocable<F &, const T &> &&
    std::convertible_to<std::invoke_result_t<F &, const T &>, T>;

  //! Computes the Nth derivative of a function by finite difference
  /*!
  @tparam N Order of the derivative.
  @tparam F Callable object type.
  @tparam T Argument and return type of the callable object.
  @tparam DT Difference scheme: DifferenceType::FORWARD,
             DifferenceType::BACKWARD, or DifferenceType::CENTERED.
   */
  template <unsigned N, typename F, typename T = double,
      typename DT = DifferenceType::FORWARD>
    requires(DifferenceType::Valid<DT> && DifferentiableFunction<F, T>)
  class NthDerivative
  {
  public:
    //! Previous derivative order (difference type may switch).
    using PreviousDerivative = NthDerivative<N - 1, F, T, typename DT::otherType>;
    //! Constructor.
    /*!
    \param f Callable object.
    \param h Step used by finite-difference formulas.
     */
    explicit NthDerivative(F f, T h) : p_derivative_{std::move(f), h}, h_{h} {}

    //! Computes the derivative at the requested point.
    /*!
    \param x Point where the derivative is computed.
    \return Approximation of the derivative.
     */
    T
    operator()(const T &x) const
    {
      if constexpr(std::same_as<DT, DifferenceType::FORWARD>)
          return (p_derivative_(x + h_) - p_derivative_(x)) / h_;
      else if constexpr(std::same_as<DT, DifferenceType::BACKWARD>)
          return (p_derivative_(x) - p_derivative_(x - h_)) / h_;
      else
        return (p_derivative_(x + h_) - p_derivative_(x - h_)) / (T{2} * h_);
    }

  private:
    PreviousDerivative p_derivative_;
    T                  h_;
  };

  /*!
   * Specialization for first derivative
   * @tparam F Callable object type.
   * @tparam T Variable type.
   * @tparam DT Difference type.
   */
  template <typename F, typename T, typename DT>
    requires(DifferenceType::Valid<DT> && DifferentiableFunction<F, T>)
  class NthDerivative<1u, F, T, DT>
  {
  public:
    //! Constructor.
    explicit NthDerivative(F f, T h) : f_{std::move(f)}, h_{h} {}
    T
    operator()(const T &x) const
    {
      if constexpr(std::same_as<DT, DifferenceType::FORWARD>)
          return (std::invoke(f_, x + h_) - std::invoke(f_, x)) / h_;
      else if constexpr(std::same_as<DT, DifferenceType::BACKWARD>)
          return (std::invoke(f_, x) - std::invoke(f_, x - h_)) / h_;
      else
        return (std::invoke(f_, x + h_) - std::invoke(f_, x - h_)) /
               (T{2} * h_);
    }

  private:
    F f_;
    T h_;
  };

  //! Only for consistency: 0th derivative
  //!
  //! It avoids infinite loops if someone creates
  //! a 0-th derivative object.
  //!
  template <typename F, typename T, typename DT>
    requires(DifferenceType::Valid<DT> && DifferentiableFunction<F, T>)
  class NthDerivative<0u, F, T, DT>
  {
  public:
    //! Constructor.
    explicit NthDerivative(F f, [[maybe_unused]] T h) : f_{std::move(f)} {}
    //! The call operator returns the value of the function.
    T
    operator()(const T &x) const
    {
      return std::invoke(f_, x);
    }

  private:
    F f_;
  };


  //! @brief Utility to simplify creation of a forward Nth-derivative object.
  /*!
   *@details  Example of usage:
   *  Example of usage:
   *  /code
   *  auto f = [](double x){ return x * std::sin(x); };
   *  double h =0.1;
   *  auto d = apsc::makeForwardDerivative<3>(f,h);
   *  auto value = d(3.0);// 3rd derivative at 3.0
   *  /endcode
   *
   * @param f a callable function with the right signature
   * @param h the step for computing derivatives
   */
  template <unsigned N, typename F, typename T>
  auto
  makeForwardDerivative(F &&f, T h)
  {
  // The use of std::decay_t allows us to strip away references and cv-qualifiers
  // from the types of F and T, ensuring that we work with the underlying types.
    using Function = std::decay_t<F>;
    using ValueType = std::decay_t<T>;
    return NthDerivative<N, Function, ValueType, DifferenceType::FORWARD>{
      std::forward<F>(f), std::move(h)};
  }

  //! @brief Utility to simplify creation of a backward Nth-derivative object.
  /*!
   *  @details  
   * Example of usage:
   *  /code
   *  auto f = [](double x){ return x * std::sin(x); };
   *  double h =0.1;
   *  auto d = apsc::makeBackwardDerivative<3>(f,h);
   *  auto value = d(6.0) //3rd derivative at 6.0
   *  /endcode
   * @param f a callable function with the right signature
   * @param h the step for computing derivatives
   */
  template <unsigned N, typename F, typename T>
  auto
  makeBackwardDerivative(F &&f, T h)
  {
    // The use of std::decay_t allows us to strip away references and cv-qualifiers
    // from the types of F and T, ensuring that we work with the underlying types.
    using Function = std::decay_t<F>;
    using ValueType = std::decay_t<T>;
    return NthDerivative<N, Function, ValueType, DifferenceType::BACKWARD>{
      std::forward<F>(f), std::move(h)};
  }
  //! @brief Utility to simplify creation of a centered Nth-derivative object.
  /*!
   * @details  
   * Example of usage:
   *  /code
   *  auto f = [](double x){ return x * std::sin(x); };
   *  double h =0.1;
   *  auto d = apsc::makeCenteredDerivative<3>(f,h);
   *  auto value = d(6.0) //3rd derivative at 6.0
   *  /endcode
   * @param f a callable function with the right signature
   * @param h the step for computing derivatives
   */
  template <unsigned N, typename F, typename T>
  auto
  makeCenteredDerivative(F &&f, T h)
  {
    // The use of std::decay_t allows us to strip away references and cv-qualifiers
    using Function = std::decay_t<F>;
    using ValueType = std::decay_t<T>;
    return NthDerivative<N, Function, ValueType, DifferenceType::CENTERED>{
      std::forward<F>(f), std::move(h)};
  }

  /*!
   * @brief Recursive lambda for computing Nth centered derivatives.
   * @details Recursive lambda utility.
   *
   * It computes the Nth centered derivative as a lambda expression.
   *
   * Example of use:
   *
   * auto f = [](double x){ return std::sin(x); };
   * auto d4 = apsc::derive<4>(f,1.e-4);
   * auto d  = d4(3.4); // fourth derivative of f at 3.4
   *
   * @note Taken from Discovering Modern C++, II Edition, by Peter Gottschling
   * @param N Order of the derivative.
   * @param f Callable object.
   * @param h Step size for finite difference approximation.
   * @return A lambda function that computes the Nth centered derivative of f at a given point.
   */

  template<unsigned int N>
  constexpr auto derive = [](auto f, auto h)
  {
    if constexpr(N == 0u)
     {
        return [f](auto x) { return f(x); };
     }
    else
      {
        auto prev = derive<N - 1u>(f, h);
        return [=](auto x) {
          return (prev(x + h) - prev(x - h)) / (decltype(h){2} * h);
        };
      }
  };

} // namespace apsc
#endif
