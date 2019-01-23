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
    R operator() (R const & x)const
    {
      //(an*x + an-1)*x + .. + a_0
      auto sum=M_coeff[N];
      for (unsigned int i = 1u;i<=N;++i)
        sum=sum*x + M_coeff[N-i];
      return sum;
    }
    //! Evaluate polynomial with Horner rule
    /*
      A more general version: now I can have real coefficient
      but x can now be a complex!.
      However if the corfficients are complex I cannot pass a double
     */
    template<class T>
    T eval(T const & x)const
    {
      //(an*x + an-1)*x + .. + a_0
      T sum=M_coeff[N];
      for (unsigned int i = 1u;i<=N;++i)
        sum=sum*x + M_coeff[N-i];
      return sum;
    }
  private:
    //! Coefficiants a_0---a_n
    std::array<R,N+1> M_coeff;  
  };
}
#endif
