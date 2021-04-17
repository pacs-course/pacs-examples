#ifndef HH_POLYNOMIALS_HH
#define HH_POLYNOMIALS_HH
#include <array>
#include <type_traits>
#include <utility>
#include <algorithm>
#include <exception>
#include <iostream>
namespace apsc::LinearAlgebra
{
  //! A simple class for "static" polynomials
  /*!
    @tpar N polynomial degree
    @tpar R polynomial field
   */
  template<unsigned int N, class R=double>
  class Polynomial
  {
  public:
    //! Default constructor (coefficients are zero initialized)
    Polynomial(){M_coeff.fill(R(0));}
    // Constructor taking coefficients
    Polynomial(const std::array<R,N+1> & c):M_coeff{c}
    {}
    //! I can initialize with another polynomial, but only if Degree<=
    template<unsigned int M>
    Polynomial(Polynomial<M,R> const & right) noexcept
    {
      static_assert(M<=N," Cannot assign a polynomial of higher degree");
      M_coeff.fill(R(0));
      std::copy(right.get_coeff().begin(),right.get_coeff().end(),M_coeff.begin());
    }
    //! For polynomial of the same type I use the implicit copy-constructore
    Polynomial(Polynomial<N,R> const &)=default;
    //! Move constructor is the implicit one
    Polynomial(Polynomial<N,R> &&)=default;
    // I can also assign a polynomial of smaller degree
    template<unsigned int M>
    Polynomial & operator =(Polynomial<M,R> const & right) noexcept
    {
      static_assert(M<=N," Cannot assign a polynomial of higher degree");
      M_coeff.fill(R(0));
      std::copy(right.get_coeff.begin(),right.get_coeff().end(),M_coeff.begin());

    }
    //! Copy assignement is the implicit one
    Polynomial & operator =(Polynomial<N,R> const &)=default;
    //! Move assignement is the implicit one
    Polynomial & operator =(Polynomial<N,R>&&)=default;
    //! Set coefficients
    void set_coeff(const std::array<R,N+1> & c)
    {
      M_coeff=c;
    }
    //! Get coefficients
    auto get_coeff()const noexcept {return M_coeff;}
    //! Get coefficient as reference (a nicer alternative to setter).
    auto & get_coeff() noexcept {return M_coeff;}

    //! Evaluate polynomial with Horner rule
    //! @param x The evaluation point
    auto constexpr operator()(R const & x)const noexcept
    {
      //(an*x + an-1)*x + .. + a_0
      auto sum=M_coeff[N];
      for (unsigned int i = 1u;i<=N;++i)
        sum=sum*x + M_coeff[N-i];
      return sum;
    }
    //! Unary minus
    auto operator -() noexcept
    {
      for (unsigned int i = 0u;i<=N;++i) this->M_coeff[i]=-this->M_coeff[i];
      return *this;
    }
    //! Unary plus
     auto operator +() noexcept
     {
       return *this;
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
  /*!
   * Polynomial division
   *
   * @tparam NDegree Degree of the numerator
   * @tparam DDegree Degree of the denominator
   * @tparam R Type of polynomial scalar field
   * @param Num Numerator
   * @param Den Denominator
   * @return A pair containing Quotient and Rest of the division
   * @pre NDegree>=DDegree
   * @pre Denominator must be a complete polynomial, i.e. the coefficient of highest degree must be !=0
   */
  template <unsigned int NDegree, unsigned int DDegree, typename R>
  auto PolyDivide(Polynomial<NDegree,R> const & Num, Polynomial<DDegree,R> const & Den)
  -> std::pair<Polynomial<NDegree-DDegree,R>,Polynomial<DDegree-1u,R>>
  {
    static_assert(NDegree>=DDegree,"Polynomial Division Error: Numerator degree less than denominator");
    constexpr auto QDegree = NDegree - DDegree;
    constexpr auto RDegree = DDegree - 1u;
    // The quotient
    Polynomial<QDegree,R> Quotient;
    auto  N           = Num.get_coeff();
    auto const & D    = Den.get_coeff();
    // Extract last denominator's coefficient
    auto const & Dend = D.back();
    // It it is 0 is an error
    if (Dend==R{0})
      throw std::runtime_error("Polynomial Division Error. Denominator must be a complete polynomial");
    // get a regerence to quotient coefficients
    auto & q=Quotient.get_coeff();
    auto dN = NDegree;
    while (dN >= DDegree)
      {
        std::array<R,NDegree+1u> d; // help array
        d.fill(0);
        // d is equal D shifted right by dN - DDegree
        std::copy(D.begin(),D.end(),d.begin()+dN-DDegree);
        auto dFrac= N[dN]/Dend;
        // calculating next denominator coefficient
        q[dN-DDegree]=dFrac;
        // d is set equal d*dFrac
        std::for_each(d.begin(),d.end(),[&dFrac](auto & x){x*=dFrac;} );
        // Update N
        //for (auto i= 0u; i<=dN; ++i)
        //  N[i]-=d[i];
        // Using transform instead of the loop
        std::transform(N.begin(),N.end(),d.begin(),N.begin(),[](auto const & n, auto const & d){return n-d;});
        // Reduce dN by one (I could have replaced the while with a for loop!
        dN--;
      }
    // The rest is in N, I have to copy into the coefficients
    Polynomial<RDegree,R> Rest;
    std::copy(N.begin(),N.begin()+RDegree+1u,Rest.get_coeff().begin());
    return std::make_pair(Quotient,Rest);
  }

  /*!
   * Polynomial division. Overload for the division by a constant polynomial (degree=0)
   * @tparam NDegree Degree of the numerator
   * @tparam R The type of the polynomial scalar field
   * @param Num The numerator
   * @param Den The denominator (of degree 0)
   * @return A pair containing Quotient and Rest of the division
   * @pre Denominator can't be the zero polynomial
   */
  template <unsigned int NDegree, typename R>
  auto PolyDivide(Polynomial<NDegree,R> const & Num, Polynomial<0u,R> const & Den)
  -> std::pair<Polynomial<NDegree,R>,Polynomial<0u,R>>
  {
    constexpr auto QDegree = NDegree;
    constexpr auto RDegree = 0u;
    Polynomial<QDegree,R> Quotient;
    Polynomial<RDegree,R> Rest;
    auto  const & N = Num.get_coeff();
    auto        & Q = Quotient.get_coeff();
    auto const Dend = Den.get_coeff().back();
    if (Dend==R(0))
      throw std::runtime_error("Polynomial Division Error. Denominator must be a complete polynomial");
    for (auto i=0u; i<Q.size();++i)Q[i]=N[i]/Dend;
    return std::make_pair(Quotient,Rest);
  }

  /*!
   * Outputs the polynomial in a pretty-print way
   * @tparam NDegree The degree
   * @tparam R The field
   * @param out The output stream
   * @param p The polynomial
   * @return The stream
   */
  template <unsigned int NDegree, typename R>
  std::ostream & operator<<(std::ostream & out, Polynomial<NDegree,R>const & p)
  {
    out<<p.get_coeff().front();
    for (auto i=1u;i<=p.degree();++i)
      {
        out<<"+"<<p.get_coeff()[i]<<"x^"<<i;
      }
    return out;
  }

  /*!
   * Simple Division. Rest is discarded
   * @param Num Numerator
   * @param Den Denominator
   * @return The result of the division (rest is discarded)
   * * @pre NDegree>=DDegree
   * @pre Denominator must be a complete polynomial, i.e. the coefficient of highest degree must be !=0
   */
  template <unsigned int NDegree, unsigned int DDegree, typename R>
  auto operator / (Polynomial<NDegree,R> const & Num, Polynomial<DDegree,R> const & Den)
  {
    auto [quot,rest] = polyDivide(Num,Den);
    return quot;
  }
  /*!
    * The rest of the division. The quationt is discarded.
    * @param Num Numerator
    * @param Den Denominator
    * @return The rest of the division (quotient is discarded)
    * * @pre NDegree>=DDegree
    * @pre Denominator must be a complete polynomial, i.e. the coefficient of highest degree must be !=0
    */

  template <unsigned int NDegree, unsigned int DDegree, typename R>
   auto operator % (Polynomial<NDegree,R> const & Num, Polynomial<DDegree,R> const & Den)
   {
     auto [quot,rest] = polyDivide(Num,Den);
     return rest;
   }

/*!
 * Polinomial addition
 * @return
 */
  template <unsigned int LDegree, unsigned int RDegree, typename R>
    auto operator + (Polynomial<LDegree,R> const & left, Polynomial<RDegree,R> const & right) noexcept
    {
     constexpr unsigned int NMAX=std::max(LDegree,RDegree);
     constexpr unsigned int NMIN=std::min(LDegree,RDegree);
     Polynomial<NMAX,R> res;
     if constexpr (NMAX==LDegree)
         {
         for (unsigned int i = NMIN+1;i<=NMAX;++i) res.get_coeff()[i]=left.get_coeff()[i];
         }
     else if (NMAX==RDegree)
       {
         for (unsigned int i = NMIN+1;i<=NMAX;++i) res.get_coeff()[i]=right.get_coeff()[i];
       }
     for (unsigned int i = 0u;i<=NMIN;++i) res.get_coeff()[i]=right.get_coeff()[i]+left.get_coeff()[i];
     return res;
   }

  /*!
   * Polinomial subtraction
   */
  template <unsigned int LDegree, unsigned int RDegree, typename R>
  auto operator - (Polynomial<LDegree,R> const & left, Polynomial<RDegree,R> const & right) noexcept
  {
    constexpr unsigned int NMAX=std::max(LDegree,RDegree);
     constexpr unsigned int NMIN=std::min(LDegree,RDegree);
     Polynomial<NMAX,R> res;
     if constexpr (NMAX==LDegree)
         {
         for (unsigned int i = NMIN+1;i<=NMAX;++i) res.get_coeff()[i]=left.get_coeff()[i];
         }
     else if (NMAX==RDegree)
       {
         for (unsigned int i = NMIN+1;i<=NMAX;++i) res.get_coeff()[i]=-right.get_coeff()[i];
       }
     for (unsigned int i = 0u;i<=NMIN;++i) res.get_coeff()[i]=left.get_coeff()[i]-right.get_coeff()[i];
     return res;
  }

  /*!
   * Multiplication of 2 polynomials
   */
  template <unsigned int LDegree, unsigned int RDegree, typename R>
  auto operator * (Polynomial<LDegree,R> const & left, Polynomial<RDegree,R> const & right) noexcept
   {
    constexpr unsigned int NRES=LDegree*RDegree;
    Polynomial<NRES,R> res;
    for (unsigned int i = 0u;i<=LDegree;++i)
      {
        for (unsigned int j = 0u;j<=RDegree;++j)
        res.get_coeff()[i*j] += left.get_coeff()[i]*right.get_coeff()[j];
      }
    return res;
   }

  /*!
   * (pre)-Multiplication with a scalar
   */
  template <unsigned int RDegree, typename R>
  auto operator * (R const & scalar, Polynomial<RDegree,R> const & right) noexcept
  {
    Polynomial<RDegree,R> res;
    for (unsigned int j = 0u;j<=RDegree;++j)
      res.get_coeff()[j]=scalar*right.get_coeff()[j];
    return res;
  }


 //! Stuff not for the general user
  namespace internals
  {
    /*!
     * A struct to support powers of polynomyals
     * It uses template recursion
     * @tparam Exp The exponent
     */
  template <unsigned int RDegree, typename R,unsigned int Exp>
  struct Pow
  {
   auto operator()(Polynomial<RDegree,R> const & p)const
    {
     Pow<RDegree,R,Exp-1u> next;
     return next(p)*p;
    }
  };

  //! Specialization for exponent equal 1
  template <unsigned int RDegree, typename R>
  struct Pow<RDegree,R,1u>
  {
    Polynomial<RDegree,R> operator()(Polynomial<RDegree,R> const & p)const
    {
     return p;
    }
  };

  //! Specialization for Polynomial od degree 0
  template <unsigned int RDegree, typename R>
  struct Pow<RDegree,R,0u>
  {
    Polynomial<0u,R> operator()(Polynomial<RDegree,R> const & p)
    {
     return Polynomial<0u,R>{{R(1)}};
    }
  };
  }// end internals


  /*!
   * Power of a polynomial
   *
   * Usage: pow<N>(p) (N>=1)
   * @tparam Exp The exponent
   * @tparam RDegree The degree of the polynomial
   * @tparam R The scalar field
   * @param p The polynomial
   * @return p^Exp
   */
  /*template <unsigned int Exp,unsigned int RDegree, typename R>
  auto pow(Polynomial<RDegree,R> const & p)
  {
    internals::Pow<RDegree,R,Exp> compute;
    return compute(p);
  };
*/

// A simpler version (since c++17)
  template <unsigned int Exp,unsigned int RDegree, typename R>
    auto pow(Polynomial<RDegree,R> const & p)
    {
    if constexpr (Exp==0u)
      return Polynomial<0u,R>{{R(1)}};
    else if constexpr (Exp==1u)
      return p;
    else
      return p*pow<Exp-1u>(p);
    };

  /*!
     * Derivative of a Polynomial
     * Usage: der<N>(p) (N>=0)
     *
     * @author Sergio Mauricio Vanegas Arias
     * @tparam M The derivative order
     * @tparam RDegree The degree of the polynomial
     * @tparam R The scalar field
     * @param p The polynomial
     * @return \f$\frac{d^{M}(p)}{dx^{M}}\f$
     */
    template <unsigned M, unsigned RDegree, typename R>
    auto der(Polynomial<RDegree, R> const & p)
    {
      if constexpr (M == 0u)
        return p;
      else if constexpr (RDegree < M)
        return Polynomial<0u,R>{{R(0)}};
      else{
        std::array<R, RDegree> C;
        for(size_t i = 1; i <= RDegree; ++i)
          C[i-1] = i * p.get_coeff()[i];
        return der<M-1>(Polynomial<RDegree-1, R>{C});
      }
    };

}
#endif
