#include <iostream>
#include <cmath>
#include <complex>
#include <array>
//! \file magnitude.hpp
/*!
  A simple example of use of traits (which are also policies in this case)

*/

//! The primary template
template <typename T>
struct Value_Trait
{
  //! The type
  using type=T;
  //! Inner product. It works for all POD
  static type prod(T const & x, T const & y)
  {return x*y;}
};

//! Specialization for complex
template <class T>
struct Value_Trait<std::complex<T> >
{
  //! The type of the complex field
    using type = T;
  //! The redefinition of the product
    static type prod(std::complex<T> const & x,std::complex<T> const & y)
      {return x.imag()*y.imag()+x.real()*y.real();}
};

//! A function using traits. It computes the inner product.
/*!
 * We need a trait to identify the correct return type, which should
 * be a T if T is a POD, and C if T is a complex::<C>. We need a
 * policy (stored in the same trait) to operate on each element of the
 * array, since the operation is different for complex than for other
 * types.
 *
 * @param x an array of values
 * @param y an array of values
 * @return the Euclidean inner product (x,y)
 */
template <typename T, std::size_t N>
auto // C++11 onwards
inline innerProduct(const std::array<T,N>& x, const std::array<T,N> & y)
{
  // Here I use the trait!
    using valueType=typename Value_Trait<T>::type;
    valueType res{0};// 0 is convertible to any POD
    for (std::size_t i=0u;i<N;++i)
      {
        // Here I use the trait!
        res+=Value_Trait<T>::prod(x[i],y[i]);
      }
    return res;
}
//! A function with automatic deduction of return type
/*
 *  It works with double, float, but also std::complex!
 *
 *  @param x a POD or complex number
 *  @param y b POD or complex number
 *  @return the argument of max(|a|,|b|)
 */
template<class T>
auto max_modulo(T const & a, T const& b )
{
  auto xa=std::abs(a);
  auto xb=std::abs(b);
  return xa>xb? a:b;
}

int main () 
{
    using namespace std;

    std::array<double,2> xr={{3.,4.}};
    std::array<double,2> yr={{6.,7.}};
    // Note the extra parenthesis. Necessary for the array
    std::array<std::complex<double>,2> xc={{{3.,4.},{-1.0,4.5}}};
    std::array<std::complex<double>,2> yc={{{6.,7.},{0.0,1.0}}};

    cout << "innerproduct of vectors = " << innerProduct(xr, yr) << '\n';
    cout << "innerproduct of ectors of complex= " << innerProduct(xc, yc) << '\n';
    cout << "argmax("<<xc[0]<<","<<yc[0]<<")="<<max_modulo(xc[0],yc[0])<<"\n";
}
