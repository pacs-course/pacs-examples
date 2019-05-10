#ifndef HH_POLYNOMIALS_HH
#define HH_POLYNOMIALS_HH
#include <array>
#include <type_traits>
namespace LinearAlgebra
{
  //! A class for polynomials
  /*!
    @tpar N polynomial degree
    @tpar R polynomial field
   */
  template<unsigned int N, class R=double>
  class Polynomial
  {
  public:
    //! Default constructor
    Polynomial()=default;
    // Constructor taking coefficients
    Polynomial(const std::array<R,N+1> & c):M_coeff{c}
    {}
    //! Set coefficients
    void set_coeff(const std::array<R,N+1> & c)
    {
      M_coeff=c;
    }
    //! Get coefficients
    auto get_coeff()const {return M_coeff;}
    
    //! Evaluate polynomial with Horner rule
    /*
      A general version: now I can have coefficient
      and values of different type as long as they are convertible.
      thanks to common_type<A,B>
      But unfortunately it does not work if R=complex<Type> and T=Type. There is not
      implicit conversion Type -> complex<Type>, So in that case I need to pass the double.
      Note that I use automatic deduction of return type (since C++14) to simplify things
     */
    template<class T>
    T operator()(T const & x)const
    {
      //(an*x + an-1)*x + .. + a_0
      std::common_type_t<T,R> sum=M_coeff[N];
      for (unsigned int i = 1u;i<=N;++i)
        sum=sum*x + M_coeff[N-i];
      return sum;
    }
    //! The polynomial degree
    /*!
     *  implemented as a constexpr function since the degree is
     *  here a constant expression. It must be static since
     *  constexpr methods should be static methods.
     */
    static constexpr unsigned int degree(){return N;}
  private:
    //! Coefficients a_0---a_n
    std::array<R,N+1> M_coeff;  
  };
}
#endif
