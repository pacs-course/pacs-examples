#ifndef BUTCHERRKF_HPP
#define BUTCHERRKF_HPP

#include <exception>
#include <memory>
#include <string>
#include <vector>

/**
 * The class contains the prototype of a Butcher table for RKF methods.
 *
 * A Butcher table (or Butcher array) for an embedded RKF method is defined by a
 * matrix A and two vectors b1 and b2. Another vector, called c is required but
 * it may be computed as the row sum of A
 */
class ButcherArray
{
public:
  /// The actual Butcher table A.
  using Table_t = std::vector<std::vector<double>>;

  /**
   * Constructor.
   *
   * @param n_stages_ The total number of stages.
   * @param A_        Table A.
   * @param b1_       Array b1 (low order).
   * @param b2_       Array b2 (high order).
   * @param order_    Order of the method (the highest one).
   */
  ButcherArray(const unsigned int &       n_stages_,
               const Table_t &            A_,
               const std::vector<double> &b1_,
               const std::vector<double> &b2_,
               const unsigned int &       order_)
    : n_stages(n_stages_)
    , A(A_)
    , b1(b1_)
    , b2(b2_)
    , order(order_)
  {
    c.assign(n_stages, 0.0);

    for (std::size_t i = 1; i < n_stages; ++i)
      for (auto const &v : A[i])
        c[i] += v;
  }

  /// It's always a good practice to declare a virtual destructor for
  /// polymorphic classes.
  virtual ~ButcherArray() = default;

  /// Number of stages.
  unsigned int n_stages;

  /**
   * Store the full array even if only the part below the main
   * diagonal is different from zero, for simplicity.
   */
  Table_t A;

  /// The b1 coefficient of the Butcher array (lower order).
  std::vector<double> b1;

  /// The b2 coefficients of the Butcher array (higher order).
  std::vector<double> b2;

  /// The c coefficient of the butcher array.
  std::vector<double> c;

  /// Order to control time steps.
  unsigned int order;
};

namespace RKFScheme
{
  // Some common RK embedded schemes.

  /// Dormand-Prince scheme.
  class DormandPrince_t : public ButcherArray
  {
  public:
    DormandPrince_t()
      : ButcherArray{
          7,
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
            {{35. / 384, 0., 500. / 1113, 125. / 192, -2187. / 6784, 11. / 84}},
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

  /// RK45, the actual RK-Fehlberg scheme.
  class RK45_t : public ButcherArray
  {
  public:
    RK45_t()
      : ButcherArray{
          6,
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
  class RK23_t : public ButcherArray
  {
  public:
    RK23_t()
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

  /// Fehlberg RK12 scheme.
  class FehlbergRK12_t : public ButcherArray
  {
  public:
    FehlbergRK12_t()
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

  /// Heun-Euler scheme, 2nd order.
  class RK12_t : public ButcherArray
  {
  public:
    RK12_t()
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

  std::unique_ptr<ButcherArray>
  make_Butcher_array(const std::string &type)
  {
    if (type == "DormandPrince")
      return std::make_unique<DormandPrince_t>();
    else if (type == "RK45")
      return std::make_unique<RK45_t>();
    else if (type == "RK23")
      return std::make_unique<RK23_t>();
    else if (type == "FehlbergRK12")
      return std::make_unique<FehlbergRK12_t>();
    else if (type == "RK12")
      return std::make_unique<RK12_t>();
    else
      throw std::invalid_argument("Invalid argument: wrong solver \"" + type +
                                  "\" specified.");
  }

} // namespace RKFScheme

#endif /* BUTCHERRKF_HPP */
