#ifndef BUTCHERRKF_HPP
#define BUTCHERRKF_HPP

#include <array>

//! The class containing the prototype of a Butcher table for RKF
//! methods
/*!
 * A Butcher table (or Butcher array) for an embedded Runge Kutta
 * method is defined by a matrix A and two vectors b1 and b2. Another
 * vector, called c is required but it may be computed as the row sum
 * of A
 *
 * \tpar NSTAGES The total number of stages
 */
template <unsigned int NSTAGES>
struct ButcherArray
{
  //! The actual Butcher array A
  using Atable = std::array<std::array<double, NSTAGES>, NSTAGES>;

  //! The constructor
  /*
   * @param a The array A
   * @param b1 The array b1 (lower order)
   * @param b2 The array b2 (higher order)
   * @param ord The order of the method (the highest one!).
   */
  ButcherArray(Atable const &                     a,
               std::array<double, NSTAGES> const &b1,
               std::array<double, NSTAGES> const &b2,
               int                                ord)
    : A(a)
    , b1(b1)
    , b2(b2)
    , order(ord)
  {
    c.fill(0.0);
    // loop over rows of A
    for (std::size_t i = 1; i < NSTAGES; ++i)
      // Loop over cols of A
      for (auto const &v : A[i])
        c[i] += v;
  }

  /* I store the full array even if only the part below the main
   * diagonal is different from zero. For simplicity
   */
  Atable A;

  //! The b1 coefficient of the Butcher array (lower order)
  std::array<double, NSTAGES> b1;

  //! The b2 coefficients of the Butcher array (higher order)
  std::array<double, NSTAGES> b2;

  //! The c coefficient of the butcher array
  std::array<double, NSTAGES> c;

  //! I need the order to control time steps
  int order;

  //! The number of steps.
  /*!
   * As a constexpr function since I need it to be resolved at compile
   * time! It is static because is a method of the class, not of a
   * specific object so I may call it with ButcherArray<N>::Nstages()
   * without creating an object of type ButcherArray
   */
  static constexpr unsigned int
  Nstages()
  {
    return NSTAGES;
  }
};

namespace RKFScheme
{
  // SOME COMMON RK embedded schemes
  //! RK45, the actual RK-Fehlberg scheme
  struct RK45_t : public ButcherArray<6>
  {
    RK45_t()
      : ButcherArray<6>{
          {{{{0., 0., 0., 0., 0., 0.}},
            {{1. / 4, 0., 0., 0., 0., 0.}},
            {{3. / 32, 9. / 32, 0., 0., 0., 0.}},
            {{1932. / 2197, -7200. / 2197, 7296. / 2197, 0., 0., 0.}},
            {{439. / 216, -8., 3680. / 513, -845. / 4104, 0., 0.}},
            {{-8. / 27,
              2.,
              -3544. / 2565,
              1859. / 4104,
              -11. / 40,
              0.}}}},
          {{25. / 216,
            0.,
            1408. / 2565,
            2197. / 4104,
            -1. / 5,
            0.}}, // 4th order
          {{16. / 135,
            0,
            6656. / 12825,
            28561. / 56430,
            -9. / 50,
            2. / 55}}, // 5th order
          5}
    {}
  };

  //! RK23 a lower order scheme
  struct RK23_t : public ButcherArray<4>
  {
    RK23_t()
      : ButcherArray<4>{
          {{{{0., 0., 0., 0.}},
            {{1. / 2, 0., 0., 0.}},
            {{0., 3. / 4, 0., 0.}},
            {{2. / 9, 1. / 3, 4. / 9, 0.}}}},
          {{7. / 24, 1. / 4, 1. / 3, 1. / 8}}, // 2nd order
          {{2. / 9, 1. / 3, 4. / 9, 0.}},      // 3rd order
          3}
    {}
  };

  //! Heun-Euler scheme 2nd order
  struct RK12_t : public ButcherArray<2>
  {
    RK12_t()
      : ButcherArray<2>{{{
                          {{0., 0.}},
                          {{1., 0.}},
                        }},
                        {{1.0, 0.0}}, // 1st order
                        {{0.5, 0.5}}, // 2nd order
                        2}
    {}
  };

  // C++17 here!
  inline const RK45_t RK45{};

  inline const RK23_t RK23{};

  inline const RK12_t RK12{};

} // namespace RKFScheme

#endif /* BUTCHERRKF_HPP */
