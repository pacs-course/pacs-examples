#ifndef _MEANEXAMPLE_H
#define _MEANEXAMPLE_H
#include <type_traits>
#include <vector>
#include <array>
namespace Utility
{
//! A simple template for the mean
/*!
  I use constexpr so it is more efficient if I pass constant expressions
  And I use automatic deduction of return type 
  @param a left operand
  @param b right operand
  @return The meean as common type of the operands
  @note if arguments are integers it returns a double
*/
template <class T>
constexpr auto
mean(T const &a, T const &b)
{
  return 0.5 * (a + b);
}
//! Overloading for pointers
template <class T>
constexpr auto
mean(T *const &a, T *const &b)
{
  return 0.5 * (*a + *b);
}

//! Overloading for vectors
template <class T>
auto
mean(std::vector<T> const &a, std::vector<T> const &b)
{
  // If the arguments are vectors of integers, we return a vector of double!
  using RetType = std::common_type_t<double, T>;
  std::vector<RetType> res(a.size());
  // if I compile woth -fopenmp I have this loop in parallel
#pragma omp parallel for 
  for(std::size_t i = 0; i < a.size(); ++i)
    res[i] = mean(a[i], b[i]);
  return res;
}

//! Overloading for arrays
template <class T, std::size_t N>
constexpr auto
mean(std::array<T,N> const &a, std::array<T,N> const &b)
{
  // If the arguments are vectors of integers, we return a vector of double!
  using RetType = std::common_type_t<double, T>;
  std::vector<RetType> res(a.size());
  for(std::size_t i = 0; i < a.size(); ++i)
    res[i] = mean(a[i], b[i]);
  return res;
}



//! Example use of variadic templates (only for nerds!)
/*!
  A function that computes the sum. First the specialization for a
  single argument
*/
template <typename T>
T
Sum(const T &a)
{
  return a;
}
//! Full template for the sum
template <typename T, typename... Ts>
std::common_type_t<T, Ts...>
Sum(const T &a, const Ts &... Args)
{
  return a + Sum(Args...);
}

/*! An example to compute the mean
  A function that computes the mean.
  I want to return at least a double (if all args are integers for instance)
*/
template <typename... Ts>
// typename std::common_type<double, Ts...>::type // old style!
std::common_type_t<double, Ts...>
Mean(const Ts &... Args)
{
  return Sum(Args...) /
         static_cast<std::common_type_t<double, Ts...>>(
           sizeof...(Ts));
}

} // namespace Utility
#endif
