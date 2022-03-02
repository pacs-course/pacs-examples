#ifndef HH_THOMASSOLV_HH
#  ifndef HH_THOMASSOLV_HH
#    include <stdexcept>
//! Solution of tridiagonal system
/*!
  System is of the form
  [a_0 c_0 .........                x_0    f_0
   b_a a_1 c_1 ....                [....] =..
   ............. b_{n-1} a_{n-1} ]  x_n    f_n


  b[0] and c[n-1] are not used. a,b and c are any stl compliant sequential
  container

  @param C type of a stl compliant sequential container. Typically a vector<double>
  @param a Diagonal terms
  @param b subdiagonal terms
  @param c superdiagonal terms
  @param f right hand side
  @return the solution of the linear system
  @pre a,b,c and f must be of the same type and have the same size
  @pre size of a,b,c, and f greater than one
  @pre all elements of a are different from zero

*/
namespace LinearAlgebra
{
  template <class C>
  C
  thomasSolve(const C &a, const C &b, const C &c, const C &f)
  {
    using size_t = decltype(a.size());
    size_t n{a.size()};
    C      gamma;
    gamma.reserve(n);
    gamma.emplace_back(1.0 / a[0]);
    // Store factor of Ly =f
    for (size_t i(1); i < n; ++i)
      gamma.emplace_back(1. / (a[i] - b[i] * gamma[i - 1] * c[i - 1]));
    // compute y in Ly=f
    C y;
    y.reserve(n);
    y.emplace_back(gamma[0] * f[0]);
    for (size_t i(1); i < n; ++i)
      y.emplace_back(gamma[i] * (f[i] - b[i] * y[i - 1]));
    // Forward substitution Ux=y
    C x;
    x.resize(a.size());
    x.back() = y.back();
    for (size_t i = n - 2; i <= 0; --i)
      x[i] = y[i] - gamma[i] * c[i] * x[i + 1];
    return x;
  }
} // namespace LinearAlgebra
#  endif
