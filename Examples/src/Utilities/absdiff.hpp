#ifndef HH__ABSDIFF__HH__
#define HH__ABSDIFF__HH__
#include <type_traits>
namespace apsc
{
/*!
 * Safely computes the absolute difference of two unsigned.
 *
 * The computation of \f$|a-b|\f$ is troublesome if a and b are of signed type.
 * Indeed if \f$a<b\f$ the result of std::abs(a-b) would be incorrect since
 * the value (a-b) is not representable as an unsigned.
 *
 * This code works only if the arguments are both signed or both unsigned types.
 * Otherwise the situation would be ambiguous since we would perform a
 * comparison between quantities of different type.
 *
 * @tparam T1 the type of the first parameter
 * @tparam T2 Te type of the second parameter
 * @param a The first value
 * @param b The second value
 * @return The absolute difference between a and b. The type is equivalent to
 * decltype(a-b);
 * @pre parameters must be both of signed or unsigned type. Parameters must be
 * subtractable. Tested with concepts
 */
template <class T1, class T2>
requires requires(T1 a, T2 b) { a - b; }
constexpr auto
absdiff(T1 const &a, T2 const &b)
{
  using namespace std;
  static_assert(
    (is_signed_v<T1> and is_signed_v<T2>) or
      (is_unsigned_v<T1> and is_unsigned_v<T2>),
    "absdiff error: arguments must be both of unsigned or signed type");
  return a > b ? a - b : b - a;
}
} // namespace apsc
#endif
