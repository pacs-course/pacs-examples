#ifndef BUTCHERRKF_HPP
#define BUTCHERRKF_HPP

#include <vector>

//! The class containing the prototype of a Butcher table for RKF
//! methods
/*!
 * A Butcher table (or Butcher array) for an embedded Runge Kutta
 * method is defined by a matrix A and two vectors b1 and b2. Another
 * vector, called c is required but it may be computed as the row sum
 * of A
 */
struct ButcherArray
{
  //! The actual Butcher array A
  using Atable = std::vector<std::vector<double>>;

  //! Default constructor is deleted.
  ButcherArray() = delete;

  //! The number of stages.
  unsigned int Nstages;

  /* I store the full array even if only the part below the main
   * diagonal is different from zero. For simplicity
   */
  Atable A;

  //! The b1 coefficient of the Butcher array (lower order)
  std::vector<double> b1;

  //! The b2 coefficients of the Butcher array (higher order)
  std::vector<double> b2;

  //! The c coefficient of the butcher array
  std::vector<double> c;

  //! I need the order to control time steps
  int order;

protected:
  /*
   * @param a The array A
   * @param b1 The array b1 (lower order)
   * @param b2 The array b2 (higher order)
   * @param ord The order of the method (the highest one!).
   */
  ButcherArray(unsigned int const &       Nstages,
               Atable const &             a,
               std::vector<double> const &b1,
               std::vector<double> const &b2,
               int                        ord)
    : Nstages(Nstages)
    , A(a)
    , b1(b1)
    , b2(b2)
    , order(ord)
  {
    c.assign(Nstages, 0.0);

    // loop over rows of A
    for (std::size_t i = 1; i < Nstages; ++i)
      // Loop over cols of A
      for (auto const &v : A[i])
        c[i] += v;
  }
};

namespace RKFScheme
{
  //! Dormand-Prince scheme
  struct DormandPrince : public ButcherArray
  {
    DormandPrince()
      : ButcherArray{7,
                     {{
                       {{0., 0., 0., 0., 0., 0.}},
                       {{1. / 5, 0., 0., 0., 0., 0.}},
                       {{3. / 40, 9. / 40, 0., 0., 0., 0.}},
                       {{44. / 45, -56. / 15, 32. / 9, 0., 0., 0.}},
                       {{19372. / 6561,
                         -25360. / 2187,
                         64448. / 6561,
                         -212. / 729,
                         0.,
                         0.}},
                       {{9017. / 3168,
                         -355. / 33,
                         46732. / 5247,
                         49. / 176,
                         -5103. / 18656,
                         0.}},
                       {{35. / 384,
                         0.,
                         500. / 1113,
                         125. / 192,
                         -2187. / 6784,
                         11. / 84}},
                     }},
                     {{5179. / 57600,
                       0.,
                       7571. / 16695,
                       393. / 640,
                       -92097. / 339200,
                       187. / 2100,
                       1. / 40}}, // 4th order
                     {{35. / 384,
                       0.,
                       500. / 1113,
                       125. / 192,
                       -2187. / 6784,
                       11. / 84,
                       0.}}, // 5th order
                     5}
    {}
  };

  // SOME COMMON RK embedded schemes
  //! RK45, the actual RK-Fehlberg scheme
  struct RK45 : public ButcherArray
  {
    RK45()
      : ButcherArray{
          6,
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
  struct RK23 : public ButcherArray
  {
    RK23()
      : ButcherArray{4,
                     {{{{0., 0., 0., 0.}},
                       {{1. / 2, 0., 0., 0.}},
                       {{0., 3. / 4, 0., 0.}},
                       {{2. / 9, 1. / 3, 4. / 9, 0.}}}},
                     {{7. / 24, 1. / 4, 1. / 3, 1. / 8}}, // 2nd order
                     {{2. / 9, 1. / 3, 4. / 9, 0.}},      // 3rd order
                     3}
    {}
  };

  //! Fehlberg RK12 scheme
  struct FehlbergRK12 : public ButcherArray
  {
    FehlbergRK12()
      : ButcherArray{3,
                     {{
                       {{0., 0., 0.}},
                       {{1. / 2, 0., 0.}},
                       {{1. / 256, 255. / 256, 0.}},
                     }},
                     {{1. / 256, 255. / 256, 0.}},       // 1st order
                     {{1. / 512, 255. / 256, 1. / 512}}, // 2nd order
                     2}
    {}
  };

  //! Heun-Euler scheme 2nd order
  struct RK12 : public ButcherArray
  {
    RK12()
      : ButcherArray{2,
                     {{
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
