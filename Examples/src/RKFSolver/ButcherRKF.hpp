/*
 * ButcherRKF.hpp
 *
 *  Created on: Feb 3, 2019
 *      Author: forma
 */

#ifndef SRC_RK45_BUTCHERRKF_HPP_
#define SRC_RK45_BUTCHERRKF_HPP_
#include <algorithm>
#include <array>
#include <concepts>
#include <numeric>
namespace apsc
{
//! The class containing the prototype of a Butcher table for RKF methods
/*!
 * A Butcher table (or Buttcher array) for an embedded Runge Kutta method is
 * defined by a matrix A and two vectors b1 and b2. Another vector, called c is
 * required but it may be computed as the row sum of A
 *
 * \tpar NSTAGES The total number of stages
 */
template <unsigned int NSTAGES> struct ButcherArray
{
  //! The actual Butcher array A
  using Atable = std::array<std::array<double, NSTAGES>, NSTAGES>;
  //! The constructor
  /*
   * @param a The array A
   * @param b1 The array b1 (primal method)
   * @param b2 The array b2 (secondary method)
   * @param ord The order of the lowest order method (the order of consistency,
   * not the LTE order!).
   */
  constexpr ButcherArray(Atable const &a, std::array<double, NSTAGES> const &b1,
                         std::array<double, NSTAGES> const &b2, int ord)
    : A{a}, b1{b1}, b2{b2}, c{}, order{ord}, implicit_{false}
  {
    std::transform(A.begin(), A.end(), c.begin(), [](auto const &row) {
      return std::accumulate(row.begin(), row.end(), 0.0);
    });
    implicit_ = set_implicit<NSTAGES>();
  }
  /* I store the full array even if only the part below the main diagonal is
   * different from zero. For simplicity
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
  /*!
   * Check if it correspond to an implicit RK scheme
   * @return true if implicit
   * @todo try to do it in the constructor. I need to check if it is possible to
   * maintain it constexpr
   *
   */
  constexpr bool
  implicit() const
  {
    return implicit_;
  }

  //! The number of steps.
  /*!
   * As a constexpr function since I need it to be resolved at compile time!
   * It is static because is a method of the class, not of a specific object
   * so I may call it with ButcherArray<N>::Nstages() without creating an object
   * of type ButcherArray
   */
  static constexpr unsigned int
  Nstages()
  {
    return NSTAGES;
  }

protected:
  bool implicit_ = false;
  template <unsigned int N>
  constexpr bool
  set_implicit()
  {
    /*   if constexpr(N <= 1u)
         implicit_ = implicit_ or (A[0][0] != 0);
       else
         implicit_ = implicit_ or (A[N - 1][N - 1] != 0);
     */
    if constexpr(N <= 1u)
      return (A[0][0] != 0);
    else
      return (A[N - 1][N - 1] != 0) or set_implicit<N - 1>();
  }
};

template <typename B>
concept ButcherArrayConcept = requires(B b) {
                                b.A;
                                b.b1;
                                b.b2;
                                b.c;
                                b.order;
                                {
                                  b.implicit()
                                  } -> std::same_as<bool>;
                                {
                                  B::Nstages()
                                  } -> std::convertible_to<unsigned int>;
                              };

namespace RKFScheme
{
  // SOME COMMON RK embedded schemes
  //! RK45, the actual RK <Fehlberg scheme
  struct RK45_t : public ButcherArray<6>
  {
    constexpr RK45_t()
      : ButcherArray<6>{
          {{{{0., 0., 0., 0., 0., 0.}},
            {{1. / 4, 0., 0., 0., 0., 0.}},
            {{3. / 32, 9. / 32, 0., 0., 0., 0.}},
            {{1932. / 2197, -7200. / 2197, 7296. / 2197, 0., 0., 0.}},
            {{439. / 216, -8., 3680. / 513, -845. / 4104, 0., 0.}},
            {{-8. / 27, 2., -3544. / 2565, 1859. / 4104, -11. / 40, 0.}}}},
          {{16. / 135, 0, 6656. / 12825, 28561. / 56430, -9. / 50,
            2. / 55}}, // 5th order
          {{25. / 216, 0., 1408. / 2565, 2197. / 4104, -1. / 5,
            0.}}, // primal 4th order
          4       // order of the estimator=the lowest of the two
        }
    {}
  };
  //! RK23 a lower order scheme
  struct RK23_t : public ButcherArray<4>
  {
    constexpr RK23_t()
      : ButcherArray<4>{{{{{0., 0., 0., 0.}},
                          {{1. / 2, 0., 0., 0.}},
                          {{0., 3. / 4, 0., 0.}},
                          {{2. / 9, 1. / 3, 4. / 9, 0.}}}},
                        {{7. / 24, 1. / 4, 1. / 3, 1. / 8}}, // primal 2nd order
                        {{2. / 9, 1. / 3, 4. / 9, 0.}},      // 3rd order
                        2}
    {}
  };
  //! Heun-Euler scheme 2nd order
  struct RK12_t : public ButcherArray<2>
  {
    constexpr RK12_t()
      : ButcherArray<2>{{{
                          {{0., 0.}},
                          {{1., 0.}},
                        }},
                        {{1.0, 0.0}}, // primal 1st order
                        {{0.5, 0.5}}, // 2nd order
                        1}
    {}
  };

  //! RK1(2) scheme 2nd order
  struct RKF12_t : public ButcherArray<3>
  {
    constexpr RKF12_t()
      : ButcherArray<3>{{{
                          {{0., 0., 0.}},
                          {{1. / 2., 0., 0.}},
                          {{1. / 256., 255. / 256., 0.}},
                        }},
                        {{1.0 / 256., 255. / 256, 0.0}},      // 1st order
                        {{1. / 512., 255. / 256, 1. / 512.}}, // 2nd order
                        1}
    {}
  };

  //! ESDIRK12 scheme (Lobatto III A)
  struct ESDIRK12_t : public ButcherArray<2>
  {
    constexpr ESDIRK12_t()
      : ButcherArray<2>{{{
                          {{0., 0.}},
                          {{0., 1.0}},
                        }},
                        {{0., 1.}},   // 1st order
                        {{0.5, 0.5}}, // 2nd order
                        1}
    {}
  };

  //! ESDIRK23 scheme 3nd order
  struct ESDIRK34_t : public ButcherArray<4>
  {
    constexpr ESDIRK34_t()
      : ButcherArray<4>{
          {{
            {{0., 0., 0., 0.}},
            {{0.43586652150845899942, 0.43586652150845899942, 0, 0}},
            {{0.14073777472470619619, -0.1083655513813208000,
              0.43586652150845899942, 0.}},
            {{0.10239940061991099768, -0.3768784522555561061,
              0.83861253012718610911, 0.43586652150845899942}},
          }},
          {{0.10239940061991099768, -0.3768784522555561061,
            0.83861253012718610911, 0.43586652150845899942}}, // 3rd order
          {{0.15702489786032493710, 0.11733044137043884870,
            0.61667803039212146434, 0.10896663037711474985}}, // 4nd order
          3}
    {}
  };

  // C++17 here!
  inline constexpr RK45_t RK45;

  inline constexpr RK23_t RK23;

  inline constexpr RK12_t RK12;

  inline constexpr RKF12_t RKF12;

  inline constexpr ESDIRK12_t ESDIRK12;

  inline constexpr ESDIRK34_t ESDIRK34;

} // namespace RKFScheme

} // namespace apsc

#endif /* SRC_RK45_BUTCHERRKF_HPP_ */
