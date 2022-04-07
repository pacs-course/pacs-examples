#ifndef BUTCHERRKF_HPP
#define BUTCHERRKF_HPP

#include <array>

/**
 * The class contains the prototype of a Butcher table for RKF methods.
 *
 * A Butcher table (or Butcher array) for an embedded RKF method is defined by a
 * matrix A and two vectors b1 and b2. Another vector, called c is required but
 * it may be computed as the row sum of A
 *
 * @tpar N_STAGES The total number of stages.
 */
template <unsigned int N_STAGES>
class ButcherArray
{
public:
  /// The actual Butcher table A.
  using Table_t = std::array<std::array<double, N_STAGES>, N_STAGES>;

  /**
   * Constructor.
   *
   * @param A_     Table A.
   * @param b1_    Array b1 (low order).
   * @param b2_    Array b2 (high order).
   * @param order_ Order of the method (the highest one).
   */
  ButcherArray(const Table_t &                     A_,
               const std::array<double, N_STAGES> &b1_,
               const std::array<double, N_STAGES> &b2_,
               const unsigned int &                order_)
    : A(A_)
    , b1(b1_)
    , b2(b2_)
    , order(order_)
  {
    c.fill(0.0);

    for (std::size_t i = 1; i < N_STAGES; ++i)
      for (auto const &v : A[i])
        c[i] += v;
  }

  /**
   * Store the full array even if only the part below the main
   * diagonal is different from zero, for simplicity.
   */
  Table_t A;

  /// The b1 coefficient of the Butcher array (lower order).
  std::array<double, N_STAGES> b1;

  /// The b2 coefficients of the Butcher array (higher order).
  std::array<double, N_STAGES> b2;

  /// The c coefficient of the butcher array.
  std::array<double, N_STAGES> c;

  /// Order to control time steps.
  unsigned int order;

  /**
   * The number of steps.
   *
   * This is a constexpr function since it needs to be resolved at compile
   * time, and static because it does not depend on the class state, so it may
   * be call it as ButcherArray<N>::n_stages() without creating an object of
   * type ButcherArray.
   */
  static constexpr unsigned int
  n_stages()
  {
    return N_STAGES;
  }
};

namespace RKFScheme
{
  // Some common RK embedded schemes.

  /// RK45, the actual RK-Fehlberg scheme.
  class RK45_t : public ButcherArray<6>
  {
  public:
    RK45_t()
      : ButcherArray<6>{
          {{{{0., 0., 0., 0., 0., 0.}},
            {{1. / 4, 0., 0., 0., 0., 0.}},
            {{3. / 32, 9. / 32, 0., 0., 0., 0.}},
            {{1932. / 2197, -7200. / 2197, 7296. / 2197, 0., 0., 0.}},
            {{439. / 216, -8., 3680. / 513, -845. / 4104, 0., 0.}},
            {{-8. / 27, 2., -3544. / 2565, 1859. / 4104, -11. / 40, 0.}}}},
          {{25. / 216, 0., 1408. / 2565, 2197. / 4104, -1. / 5, 0.}}, // 4th
                                                                      // order
          {{16. / 135,
            0,
            6656. / 12825,
            28561. / 56430,
            -9. / 50,
            2. / 55}}, // 5th order
          5}
    {}
  };

  /// RK23, a lower order scheme.
  class RK23_t : public ButcherArray<4>
  {
  public:
    RK23_t()
      : ButcherArray<4>{{{{{0., 0., 0., 0.}},
                          {{1. / 2, 0., 0., 0.}},
                          {{0., 3. / 4, 0., 0.}},
                          {{2. / 9, 1. / 3, 4. / 9, 0.}}}},
                        {{7. / 24, 1. / 4, 1. / 3, 1. / 8}}, // 2nd order
                        {{2. / 9, 1. / 3, 4. / 9, 0.}},      // 3rd order
                        3}
    {}
  };

  /// Heun-Euler scheme, 2nd order.
  class RK12_t : public ButcherArray<2>
  {
  public:
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

} // namespace RKFScheme

#endif /* BUTCHERRKF_HPP */
