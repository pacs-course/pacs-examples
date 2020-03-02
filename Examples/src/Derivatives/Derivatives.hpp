#ifndef HH__DERIVATIVESHPP_HH
#define HH__DERIVATIVESHPP_HH
#include <type_traits>
#include <functional>
namespace apsc
{
  namespace DifferenceType
  {
    struct BACKWARD;// forward declaration

    struct FORWARD{
      using otherType=BACKWARD;
    };
    
    struct BACKWARD{
      using otherType=FORWARD;
    };
  }

  //! Computes the Nth derivative of a function by finite difference
  template<unsigned N, typename F, typename T=double,
           typename DT=DifferenceType::FORWARD>
  class NthDerivative
  {
  public:
    using PreviousDerivative=NthDerivative<N-1,F,T,typename DT::otherType>;
    //! Constructor
    NthDerivative(const F& f, const T & h):pDerivative{f,h}, h{h}{}
    T operator()(const T& x) const
    {
      // from C++17 constexpr if statement
      if constexpr (std::is_same<DifferenceType::FORWARD,DT>::value)
        return (pDerivative(x+h)-pDerivative(x))/h;
      else
        return (pDerivative(x)-pDerivative(x-h))/h;
    }
  private:
    PreviousDerivative pDerivative;
    T h;
  };
  

 template<typename F, typename T, typename DT>
 class NthDerivative<1u,F,T,DT>
  {
  public:
    //! Constructor
    NthDerivative(const F& f, const T & h):pDerivative{f}, h{h}{}
    T operator()(const T& x) const
    {
      if constexpr (std::is_same<DifferenceType::FORWARD,DT>::value)
        return (pDerivative(x+h)-pDerivative(x))/h;
      else
        return (pDerivative(x)-pDerivative(x-h))/h;
    }
  private:
    F pDerivative;
    T h;
  };

  //! Only for consistency: 0th derivative
  //!
  //! It avoids infinite loops if someone creates
  //! a 0-th derivative object.
  //!
  template<typename F, typename T, typename DT>
 class NthDerivative<0u,F,T,DT>
  {
  public:
    //! Constructor
    NthDerivative(const F& f, const T & h):pDerivative{f}, h{h}{}
    T operator()(const T& x) const
    {
      return pDerivative(x));
    }
  private:
    F pDerivative;
    T h;
  };

  //! Utility to simplify the creation of a Nthderivative object
  /*
   *  Example of usage: 
   *  /code
   *  auto f =[](const double & x){return x*std::sin(x);};
   *  double h =0.1;
   *  auto d = apsc::makeForwardDerivative<3>(f,h);
   *  /endcode
   *
   * /param f a callable function with the rigth signature
   * /param h the step for computing derivatives
   */
  template<unsigned N, typename F, typename T>
  auto makeForwardDerivative(const F& f, const T& h)
  {
    return  NthDerivative<N,F,T,DifferenceType::FORWARD>{f,h};
  }

 //! Utility to simplify the creation of a Nthderivative object
  /*
   *  Example of usage: 
   *  /code
   *  auto f =[](const double & x){return x*std::sin(x);};
   *  double h =0.1;
   *  auto d = apsc::makeBackwardDerivative<3>(f,h);
   *  /endcode
   * /param f a callable function with the rigth signature
   * /param h the step for computing derivatives
   */
  template<unsigned N, typename F, typename T>
  auto makeBackwardDerivative(const F& f, const T& h)
  {
    return  NthDerivative<N,F,T,DifferenceType::BACKWARD>{f,h};
  }
  
    
}
#endif
