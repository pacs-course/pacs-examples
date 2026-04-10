#ifndef HH_THOMASSOLV_HH
#define HH_THOMASSOLV_HH
#include <algorithm>
#include <complex>
#include <concepts>
#include <ranges>
#include <type_traits>
#include <utility>
#include <vector>
namespace apsc
{
namespace detail
{
  template <class T> struct is_complex : std::false_type
  {};

  template <class T> struct is_complex<std::complex<T>> : std::true_type
  {};

  template <class T>
  inline constexpr bool is_complex_v =
    is_complex<std::remove_cvref_t<T>>::value;

  template <class T>
  concept thomas_scalar =
    std::floating_point<std::remove_cvref_t<T>> || is_complex_v<T>;

  template <class R>
  concept thomas_range =
    std::ranges::forward_range<R> && std::ranges::sized_range<R> &&
    thomas_scalar<std::ranges::range_value_t<R>>;

  template <class T, std::ranges::input_range R>
  auto
  to_vector(R &&range)
  {
    return std::vector<T>(std::ranges::begin(range), std::ranges::end(range));
  }

  template <class... R>
  using common_value_t = std::common_type_t<std::ranges::range_value_t<R>...>;
} // namespace detail

//! Solution of a tridiagonal system
/*!
  System is of the form
  [a_0 c_0 .........                x_0          f_0
   b_1 a_1 c_1 ....                [....   ]  =  ..
   ............. b_{n-1} a_{n-1} ]  x_{n-1}      f_{n-1}


  b[0] and c[n-1] are not used. The inputs can be any C++20 forward, sized
  ranges whose value type is floating point or std::complex.

  @note The dimension of the system is determined by the size of a. All ranges
  must have the same size, even if some coefficients are unused.
  @note The algorithm copies the input ranges into internal working vectors, so
  the caller-provided ranges are never modified.
  @param a diagonal terms, a[0]...a[n-1]
  @param b subdiagonal terms, b[1]...b[n-1]
  @param c superdiagonal terms, c[0]...c[n-2]
  @param f right hand side, f[0]...f[n-1]
  @return The solution of the linear system as a std::vector of the common
  value type of the inputs.
  @pre a, b, c and f have the same size
  @pre size of a, b, c and f is greater than one
  @pre all elements of a are different from zero
*/
template <detail::thomas_range A, detail::thomas_range B,
          detail::thomas_range C, detail::thomas_range F>
  requires detail::thomas_scalar<detail::common_value_t<A, B, C, F>>
auto
thomasSolve(A &&a, B &&b, C &&c, F &&f)
{
  using value_type = detail::common_value_t<A, B, C, F>;

  const auto n = std::ranges::size(a);
  // Forward sweep
  auto diagonal = detail::to_vector<value_type>(std::forward<A>(a));
  auto lower = detail::to_vector<value_type>(std::forward<B>(b));
  auto upper = detail::to_vector<value_type>(std::forward<C>(c));
  auto x = detail::to_vector<value_type>(std::forward<F>(f));

  for(auto i : std::views::iota(std::size_t{1}, n))
    {
      const auto gamma = lower[i] / diagonal[i - 1];
      diagonal[i] -= gamma * upper[i - 1];
      x[i] -= gamma * x[i - 1];
    }
  // back substitution
  x.back() /= diagonal.back();
  if(n > 1u)
    {
      for(auto i :
          std::views::iota(std::size_t{0}, n - 1u) | std::views::reverse)
        {
          x[i] = (x[i] - upper[i] * x[i + 1u]) / diagonal[i];
        }
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

  b[0] and a[n-1], b[n-1], c[n-1] are not used. Inputs can be any C++20
  forward, sized ranges whose value type is floating point or std::complex.
  @note The dimension of the system is determined by the size of a:
  n = size(a). All ranges must have the same size.
  @param a diagonal terms
  @param b subdiagonal terms
  @param c superdiagonal terms
  @param f right hand side
  @return The solution of the linear system as a std::vector of the common
  value type of the inputs.
  @pre a, b, c and f have the same size
  @pre size of a, b, c and f is greater than one
  @pre all elements of a are different from zero
 */
template <detail::thomas_range A, detail::thomas_range B,
          detail::thomas_range C, detail::thomas_range F>
  requires detail::thomas_scalar<detail::common_value_t<A, B, C, F>>
auto
thomasSolveSym(A &&a, B &&b, C &&c, F &&f)
{
  using value_type = detail::common_value_t<A, B, C, F>;

  const auto n = std::ranges::size(a);
  auto       diagonal = detail::to_vector<value_type>(std::forward<A>(a));
  auto       lower = detail::to_vector<value_type>(std::forward<B>(b));
  auto       upper = detail::to_vector<value_type>(std::forward<C>(c));
  auto       rhs = detail::to_vector<value_type>(std::forward<F>(f));
  // The solution is based on a block LU decomposition of the matrix.
  // reduced system, I need to reduce only a, since it governs the size of the
  // reduces system
  auto ar = detail::to_vector<value_type>(diagonal | std::views::take(n - 1u));
  // Forward step
  // Ar  z = fr
  auto z = thomasSolve(ar, lower, upper, rhs);
  // y = f_{n-1} + z_0
  auto y = rhs[n - 1] + z[0];
  // Backsolve
  std::vector<value_type> cv(n - 1u, value_type{});
  cv[n - 2] = upper[n - 2];
  // A z = [0,...,1]^T
  z = thomasSolve(ar, lower, upper, cv);
  y /= (1 + z[0]); // last element
  std::ranges::copy(rhs | std::views::take(n - 1u), cv.begin());
  cv.back() -= upper[n - 2] * y;
  // A z = fr - c y
  z = thomasSolve(ar, lower, upper, cv);
  z.resize(n);
  z.back() = y;
  z.shrink_to_fit();
  return z;
}

//! An utility to multiply a tridiagonal matrix stored in three ranges
/*!
  Useful for validating thomasSolve()

  @param a main diagonal stored in a[0]... a[n-1]
  @param b lower diagonal stored in b[1]... b[n-1]
  @param c upper diagonal stored in c[0] c[n-2]
  @param v the vector to be multiplied with
  @param sym true is the system represents a problem with jump conditions,
  see @ref thomasSolveSym
  @return The matrix-vector product as a std::vector of the common value type
  of the inputs.

 @note The dimension of the system is determined by the size of a: n=size(a).
 Inputs can be any C++20 forward, sized ranges whose value type is floating
 point or std::complex.
*/

template <detail::thomas_range A, detail::thomas_range B,
          detail::thomas_range C, detail::thomas_range V>
  requires detail::thomas_scalar<detail::common_value_t<A, B, C, V>>
auto
matVecTrid(A &&a, B &&b, C &&c, V &&v, const bool sym = false)
{
  using value_type = detail::common_value_t<A, B, C, V>;

  auto       diagonal = detail::to_vector<value_type>(std::forward<A>(a));
  auto       lower = detail::to_vector<value_type>(std::forward<B>(b));
  auto       upper = detail::to_vector<value_type>(std::forward<C>(c));
  auto       values = detail::to_vector<value_type>(std::forward<V>(v));
  const auto n = diagonal.size();

  std::vector<value_type> res;
  res.reserve(n);
  res.emplace_back(diagonal[0] * values[0] + upper[0] * values[1]);
  for(auto i : std::views::iota(std::size_t{1}, n - 1u))
    res.emplace_back(diagonal[i] * values[i] + lower[i] * values[i - 1u] +
                     upper[i] * values[i + 1u]);
  if(sym)
    res.emplace_back(-values[0] + values[n - 1u]);
  else
    res.emplace_back(diagonal[n - 1u] * values[n - 1u] +
                     lower[n - 1u] * values[n - 2u]);
  return res;
}

} // namespace apsc
#endif
