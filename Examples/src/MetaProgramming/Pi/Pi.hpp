#ifndef HH_PICOMPUTE_HH
#define HH_PICOMPUTE_HH
// Metaprogramming trick to compute Pi

//! This namespace introduces helper functions that are used only locally
namespace apsc::internals
{
//! A template recursion for Pow. I do not want to call std::pow
template <unsigned long B, unsigned long int E> struct Pow
{
  static constexpr unsigned long int value = B * Pow<B, E - 1ul>::value;
};

template <unsigned long B> struct Pow<B, 0ul>
{
  static constexpr unsigned long int value = 1ul;
};

//! Use a truncated series to compute PI. N+1 is the number of terms
//! of the series that are being used. A nice use of static resolution
//! of recursion (N not too high)
/*!
 * @tparam R Has to be a floating point type (double or float etc.)
 */
template <class R, unsigned long int N> struct computePi
{
  static constexpr R value = static_cast<R>(1.0) /
                               internals::Pow<16, N>::value *
                               (4.0 / (8 * N + static_cast<R>(1.0)) -
                                2.0 / (8 * N + static_cast<R>(4.0)) -
                                1.0 / (8 * N + static_cast<R>(5.0)) -
                                1.0 / (8 * N + static_cast<R>(6.0))) +
                             computePi<R, N - 1>::value;
};

//! The class to close recursion
template <class R> struct computePi<R, 0ul>
{
  static constexpr R value = static_cast<R>(47) / static_cast<R>(15);
};

} // namespace apsc::internals
//! Some useful approximations of pi
namespace apsc::MathConstants
{
constexpr double      Pi = apsc::internals::computePi<double, 12ul>::value;
constexpr float       Pi_f = apsc::internals::computePi<float, 8ul>::value;
constexpr long double Pi_l =
  apsc::internals::computePi<long double, 14ul>::value;
} // namespace apsc::MathConstants

#endif
