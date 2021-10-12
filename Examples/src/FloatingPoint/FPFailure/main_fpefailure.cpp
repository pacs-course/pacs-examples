#ifndef HH_STRANGEFUNCTION_HPP
#define HH_STRANGEFUNCTION_HPP
#include "Pow.hpp"
#include <array>
#include <iomanip>
#include <iostream>
//! Rump example
/*!
  The function is \f[
    r(x,y)=Ay^6+x^2(Bx^2y^2-y^6-Cy^4-D)+Ey^8+\frac{x}{D*y}
   \f] with
  \f$A=333.75\f$, \f$B=11\f$, \f$C=121\f$, \f$D=2\f$ and \f$E=5.5\f$.

  This function is badly conditioned. In particular at \f$
  (x,y)=(77617,33096)\f$ its condition number is of the order of \f$10^32\f$.

  Thus the computed value may be strongly affected by round-off. This is indeed
  the case, even when using long double, as it can be easily assesed comparing
  the obtained value with the exact one \f$ r(77617,33096)=-0.827386\f$.

  \note We have used the apsc::pow<T>() function in Pow.hpp, which implement the
  power by an positive integer exponent by replacing it with multiplication. Not
  only because it is normally more efficient (for small exponents), but also to
  avoid to attribute (wrongly) the inaccuracy to std::pow().

  \tparam T the floating point type used for the computation (automatically
  deduced) \param x first variable \param y second variable
 */
template <class T>
auto
f(T const &x, T const &y)
{
  using namespace apsc;
  T constexpr A = 333.75;
  T constexpr B = 11;
  T constexpr C = 121;
  T constexpr D = 2;
  T constexpr E = 5.5;

  return A * pow<6>(y) +
         pow<2>(x) *
           (B * pow<2>(x) * pow<2>(y) - pow<6>(y) - C * pow<4>(y) - D) +
         E * pow<8>(y) + x / (D * y);
};

/*!
  \brief A pathological sequence

  This sequence has 6 as limit. But becouse of round off error it converges to
  100.

 */
template <class T> class Pathologic_Sequence
{
public:
  void
  reset()
  {
    current_values = initial_values;
    counter = 0;
  }
  T
  operator()()
  {
    T next;
    if(counter < 2)
      {
        next = current_values[counter];
      }
    else
      {
        constexpr T A{111.0};
        constexpr T B{1130.0};
        constexpr T C{3000.0};
        next = A - B / current_values[1] +
               C / (current_values[0] * current_values[1]);
        current_values[0] = current_values[1];
        current_values[1] = next;
      }
    ++counter;
    return next;
  }

private:
  static constexpr std::array<T, 2> initial_values{T{2.0}, T{-4.0}};
  std::array<T, 2>                  current_values = initial_values;
  unsigned                          counter{0};
};

int
main()
{
  constexpr double realSolution = -0.827396;
  std::cout << std::scientific << std::setprecision(15);
  std::cout << "   *****    RUMP EXAMPLE    ******   " << std::endl;
  std::cout << " Float solution:" << f(77617.f, 33096.f) << std::endl;
  std::cout << " Double solution:" << f(77617., 33096.) << std::endl;
  std::cout << " Long Double solution:" << f(77617.L, 33096.L) << std::endl;
  std::cout << " Exact solution (6 signif. digits):" << realSolution
            << std::endl;

  std::cout << "type anything to continue" << std::endl;
  std::string dummy;
  std::cin >> dummy;
  int                              ntrials = 100;
  Pathologic_Sequence<long double> s;
  std::cout << " Values of pathologic series (**it should converge to 6!**)"
            << std::endl;
  // plot a few values
  for(auto i = 0; i <= ntrials; ++i)
    {
      auto value = s();
      if(i <= 20 || (i % (ntrials / 20)) == 0 || (i == ntrials - 1))
        std::cout << "n: " << i << " value:" << value << std::endl;
    }
}

#endif
