#ifndef HH_THOMASSOLV_HH
#define HH_THOMASSOLV_HH
#include <algorithm>
#include <type_traits>
namespace apsc
{
//! Solution of a tridiagonal system
/*!
  System is of the form
  [a_0 c_0 .........                x_0          f_0
   b_1 a_1 c_1 ....                [....   ]  =  ..
   ............. b_{n-1} a_{n-1} ]  x_{n-1}      f_{n-1}


  b[0] and c[n-1] are not used. a,b and c can be any stl compliant sequential
  container

  @note a, b and c MUST be of the dimension of the system even is some element
  are not used <b>The dimension of the system is determined by the size of
  a</b>.
  @param a diagonal terms
  @param b subdiagonal terms
  @param c superdiagonal terms
  @param f right hand side
  @return the solution of the linear system
  @pre a,b,c and f must obey the semantics of a stl compliant sequential
  container
  @pre size of a,b,c, and f greater than one
  @pre <b>all elements of a are different from zero<\b>
 */
/*
New version: I pass the arguments by forwarding reference to avoid copies
if the arguments are rvalues
In fact only a and f are modified, so I can use std::forward<decltype(a)>(a)
and std::forward<decltype(f)>(f) to avoid copies. The other arguments are not
modified so I can use const reference
*/
auto
thomasSolve(auto &&a, auto const &b, auto const &c, auto &&f)
{
  auto n = a.size();
  // Forward sweep
  auto B = std::forward<decltype(a)>(a);
  auto D = std::forward<decltype(f)>(f);
  for(size_t i = 1; i < n; ++i)
    {
      auto gamma = b[i] / B[i - 1];
      B[i] = B[i] - gamma * c[i - 1];
      D[i] = D[i] - gamma * D[i - 1];
    }
  // back substitution
  auto x = std::move(D);
  x.back() /= B.back();
  for(int i = n - 2; i >= 0; --i)
    {
      x[i] = (x[i] - c[i] * x[i + 1]) / B[i];
    }
  return x;
}
//! Solution of tridiagonal system on a system of n equations and symmetry
//! conditions
/*!
  System is of the form

  [a_0 c_0 .........                x_0          f_0
   b_1 a_1 c_1 ....                [....   ]  =  ..
   -1.............  1 ]             x_{n-1}      f_{n-1}

which represent a problem with jump conditions x_{n-1}-x_0=f_{n-1}

  b[0] and a[n-1], b[n-1], c[n-1] are not used. a,b and c are any stl compliant
sequential container
  @note a, b and c MUST be of the dimension of the system even is some element
are not used <b>The dimension of the system is determined by the size of a:
n=a.size()</b>

  @tparam C type of a stl compliant sequential container. Typically a
vector<double>
  @param a diagonal terms
  @param b subdiagonal terms
  @param c superdiagonal terms
  @param f right hand side
  @return the solution of the linear system
  @pre a,b,c and f must be of the same type and have the same size
  @pre size of a,b,c, and f greater than one
  @pre <b>all elements of a are different from zero</b>
 */
template <class C>
C
thomasSolveSym(const C &a, const C &b, const C &c, const C &f)
{
  auto n = a.size();
  // The solution is based on a block LU decomposition of the matrix.
  // reduced system, I need to reduce only a, since it governs the size of the
  // reduces system
  C ar(a.cbegin(), a.end() - 1);
  // Forward step
  // Ar  z = fr
  auto z = thomasSolve(ar, b, c, f);
  // y = f_{n-1} + z_0
  auto y = f[n - 1] + z[0];
  // Backsolve
  C cv(n - 1, 0.);
  cv[n - 2] = c[n - 2];
  // A z = [0,...,1]^T
  z = thomasSolve(ar, b, c, cv);
  y /= (1 + z[0]); // last element
  std::copy(f.begin(), f.end() - 1, cv.begin());
  cv.back() -= c[n - 2] * y;
  // A z = fr - c y
  z = thomasSolve(ar, b, c, cv);
  z.resize(n);
  z.back() = y;
  z.shrink_to_fit();
  return z;
}

//! An utility to multiply a tridiagonal matrix strored in three vectors
/*!
  Useful for validating thomasSolve()

  @param a main diagonal stored in a[0]... a[n-1]
  @param b lower diagonal stored in b[1]... b[n-1]
  @param c upper diagonal stored in c[0] c[n-2]
  @param v the vector to be multiplied with
  @param sym true is the system represents a problem with jump conditions,
  see @ref thomasSolveSym
  @return the result

 @note   <b>The dimension of the system is determined by the size of a:
 n=a.size()</b>
*/

auto
matVecTrid(auto const &a, auto const &b, auto const &c, auto const &v,
           const bool sym = false)
{
  using size_t = decltype(a.size());
  using C = std::remove_cvref_t<decltype(v)>;
  auto n = a.size();
  C    res;
  res.reserve(n);
  res.emplace_back(a[0] * v[0] + c[0] * v[1]);
  for(size_t i = 1; i < n - 1; ++i)
    res.emplace_back(a[i] * v[i] + b[i] * v[i - 1] + c[i] * v[i + 1]);
  if(sym)
    res.emplace_back(-v[0] + v[n - 1]);
  else
    res.emplace_back(a[n - 1] * v[n - 1] + b[n - 1] * v[n - 2]);
  return res;
}

} // namespace apsc
#endif
