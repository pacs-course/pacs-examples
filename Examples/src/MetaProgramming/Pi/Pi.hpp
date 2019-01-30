#ifndef HH_PICOMPUTE_HH
#define HH_PICOMPUTE_HH
// Metaprogramming trick to compute Pi
namespace internals
{
  template<unsigned long B, unsigned long int E>
  struct Pow
  {
    static constexpr unsigned long int value=B*Pow<B,E-1ul>::value;
  };
  
  template<unsigned long B>
  struct Pow<B,0ul>
  {
    static constexpr unsigned long int value=1ul;
  };

  template<class R, unsigned long int N>
  struct
  computePi
  {
    static constexpr R value=
      static_cast<R>(1.0)/internals::Pow<16,N>::value *
      (
       4.0/(8*N + static_cast<R>(1.0)) - 2.0/(8*N + static_cast<R>(4.0)) -
       1.0/(8*N + static_cast<R>(5.0)) - 1.0/(8*N + static_cast<R>(6.0))
       )
      + computePi<R,N-1>::value;
  };

  template<class R>
  struct
  computePi<R,0ul>
  {
    static constexpr R value= static_cast<R>(47)/static_cast<R>(15);
  };
  
  
}
namespace MathConstants
{
  constexpr double Pi = internals::computePi<double,10ul>::value;
  constexpr float Pi_f= internals::computePi<float,6ul>::value;
  constexpr long double Pi_l = internals::computePi<long double,12ul>::value;
}

#endif
