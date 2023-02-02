#ifndef HH__DERIVATIVESHPP_HH
#define HH__DERIVATIVESHPP_HH
#include <functional>
#include <type_traits>
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
  } // namespace DifferenceType

  //! Computes the Nth derivative of a function by finite difference
  /*!
  \tparam N the order of the derivative
  \tparam F the callable object of signature T (T const &)
  \tparam T The argument and return type of the callable object
  \tparam DT The type of differencing: either DifferenceType::FORWARD or
  DifferenceType::BACKWARD or DifferenceType::CENTERED
   */
  template <unsigned N, typename F, typename T = double,
      typename DT = DifferenceType::FORWARD>
  class NthDerivative
  {
  public:
    //! the previous derivative, the type is switched.
    using PreviousDerivative = NthDerivative<N - 1, F, T, typename DT::otherType>;
    //! Constructor
    /*!
    \param f The function (callable object)
    \param h The spacing to be used in the formula
     */
    NthDerivative(const F &f, T h) : pDerivative{f, h}, h{h} {}
    //! The call operator that computes the derivative
    /*!
    \param x The point where the derivative is computed
    \return The derivative value
     */
    T
    operator()(const T &x) const
    {
      if constexpr(std::is_same<DifferenceType::FORWARD, DT>::value)
          return (pDerivative(x + h) - pDerivative(x)) / h;
      else if constexpr(std::is_same<DifferenceType::BACKWARD, DT>::value)
          return (pDerivative(x) - pDerivative(x - h)) / h;
      else
        return (pDerivative(x + h) - pDerivative(x - h)) / (2 * h);
    }

  private:
    PreviousDerivative pDerivative;
    T const            h;
  };

  /*!
   * Specialization for first derivative
   * @tparam F A callable object type
   * @tparam T The type of the varaible
   * @tparam DT The difference type
   */
  template <typename F, typename T, typename DT> class NthDerivative<1u, F, T, DT>
  {
  public:
    //! Constructor
    NthDerivative(const F &f, T h) : f{f}, h{h} {}
    T
    operator()(const T &x) const
    {
      //! This is C++17 syntax. Don't use a simple  if if you want efficiency!
      if constexpr(std::is_same_v<DifferenceType::FORWARD, DT>)
          return (f(x + h) - f(x)) / h;
      else if constexpr(std::is_same_v<DifferenceType::BACKWARD, DT>)
          return (f(x) - f(x - h)) / h;
      else
        return (f(x + h) - f(x - h)) / (2. * h);
    }

  private:
    // Now I store the function
    F const &f;
    T const  h;
  };

  //! Only for consistency: 0th derivative
  //!
  //! It avoids infinite loops if someone creates
  //! a 0-th derivative object.
  //!
  template <typename F, typename T, typename DT> class NthDerivative<0u, F, T, DT>
  {
  public:
    //! Constructor
    NthDerivative(const F &f, T h) : f{f}, h{h} {}
    //! The call operator returns the value of the function
    T
    operator()(const T &x) const
    {
      return f(x);
    }

  private:
    F const &f;
    T const  h;
  };


  //! Utility to simplify the creation of a Nthderivative object
  /*
   *  Example of usage:
   *  /code
   *  auto f =[](const double & x){return x*std::sin(x);};
   *  double h =0.1;
   *  auto d = apsc::makeForwardDerivative<3>(f,h);
   *  auto value = d(3.0);// 3rd derivative at 3.0
   *  /endcode
   *
   * /param f a callable function with the rigth signature
   * /param h the step for computing derivatives
   */
  template <unsigned N, typename F, typename T>
  auto
  makeForwardDerivative(const F &f, const T &h)
  {
    return NthDerivative<N, F, T, DifferenceType::FORWARD>{f, h};
  }

  //! Utility to simplify the creation of a Nthderivative object
  /*
   *  Example of usage:
   *  /code
   *  auto f =[](const double & x){return x*std::sin(x);};
   *  double h =0.1;
   *  auto d = apsc::makeBackwardDerivative<3>(f,h);
   *  auto value = d(6.0) //3rd derivative at 6.0
   *  /endcode
   * /param f a callable function with the rigth signature
   * /param h the step for computing derivatives
   */
  template <unsigned N, typename F, typename T>
  auto
  makeBackwardDerivative(const F &f, const T &h)
  {
    return NthDerivative<N, F, T, DifferenceType::BACKWARD>{f, h};
  }
  //! Utility to simplify the creation of a Nthderivative object
  /*
   *  Example of usage:
   *  /code
   *  auto f =[](const double & x){return x*std::sin(x);};
   *  double h =0.1;
   *  auto d = apsc::makeBackwardDerivative<3>(f,h);
   *  auto value = d(6.0) //3rd derivative at 6.0
   *  /endcode
   * /param f a callable function with the rigth signature
   * /param h the step for computing derivatives
   */
  template <unsigned N, typename F, typename T>
  auto
  makeCenteredDerivative(const F &f, const T &h)
  {
    return NthDerivative<N, F, T, DifferenceType::CENTERED>{f, h};
  }

  /*!
   * A lambda recursive function!
   *
   * It performs the nth centered derivative.
   *
   * Example of use:
   *
   * f = auto [](double x){return std::sin(x);}
   * auto d4 = lambda::derive<4>(f,1.e-4);
   * auto d  = f(3.4);// fourth derivative of f in 3.4
   *
   * Taken from Discovering Modern C++, II Edition, by Peter Gottschling
   *
   */

  template<unsigned int N>
  constexpr auto derive =[](auto f, auto h)
  {
    if constexpr (N==0u)
     {
        return [f](auto x){return f(x);};
     }
    else
      {
        auto prev = derive<N-1u>(f,h);
        return [=] (auto x){
        return (prev(x+h)-prev(x-h))/(2.*h);
        };
      }
  };

} // namespace apsc
#endif
