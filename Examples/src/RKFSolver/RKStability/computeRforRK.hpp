/*
 * computeRforRK.hpp
 *
 *  Created on: Nov 21, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_RKFSOLVER_COMPUTERFORRK_HPP_
#define EXAMPLES_SRC_RKFSOLVER_COMPUTERFORRK_HPP_
#include "basicZeroFun.hpp"
#include <array>
#include <complex>
#include <iostream>
#include <limits>
#include <tuple>
#include <vector>
namespace apsc
{
/*!
 * This class computes the amplification factor for a Runge-Kutta explicit
 * method. The amplification factor is the factor \f$ R(z) \f$ has the property
 * that, when applied on the model problem \f[ y^\prime(t)=\lambda y,\
 * t>0,\qquad y(0)=1, \f] with \f$\lambda\f$ a complex number, with a spacing
 * \f$ h\f$ the method produces \f[ u_{n+1} = R(z)u_{n},\qquad z=h\lambda \f]
 *
 * It contains several methods for investigating the stability region.
 *
 * The absolute stability region \f$ A\f$ is defined as
 * \f[
 * A = \{z\in C:\ |R(z)|<1 \}
 * \f]
 *
 * @tparam Butcher the type of ButcherArray (defined in ButcherRKF.hpp)
 * @tparam Level In case of Butcher array for an embedded scheme, Level=1 or 2
 * chooses the corresponding \f$b$ vector.
 */
template <typename Butcher, int Level = 1> class computeRforRK
{
public:
  using complex = std::complex<double>;
  computeRforRK(const Butcher b) : my_B(b){};
  computeRforRK() : my_B{Butcher{}} {};
  void
  setB(Butcher const &myB)
  {
    my_B = myB;
  }
  /*!
   * Computes the amplification factor \f$ R(z) \f$.
   * @param z A complex number
   * @return The amplification factor.
   */
  complex
  operator()(complex const &z) const
  {
    auto const &                           A = my_B.A;
    std::array<double, Butcher::Nstages()> b;
    if(Level == 1)
      b = my_B.b1;
    else
      b = my_B.b2;
    std::array<complex, Butcher::Nstages()> K;
    K.fill(z);
    for(unsigned int stage = 1u; stage < my_B.Nstages(); ++stage)
      {
        for(unsigned int j = 0; j < stage; ++j)
          K[stage] += z * A[stage][j] * K[j];
      }
    complex res{1., 0.};
    for(unsigned int stage = 0; stage < my_B.Nstages(); ++stage)
      res += b[stage] * K[stage];
    return res;
  }
  /*!
   * The amplification factor is, by construction, equal to 1 for z equal to
   * zero. Since I want to find the points where \f$ |R(z)|=1\f$, excluding \f$
   * z=0\f$, I compute the deflated factor \f$ (|R(z)|-1)/|z| \f$. This function
   * cannot be called with z equal to zero.
   *
   * @param z A complex number
   * @pre \f$ z\ne 0\f$
   * @return A factor that is zero when \f$ |R(z)|=1| \f$.
   */
  double
  deflatedFactor(complex const &z) const
  {
    return (std::abs(this->operator()(z)) - 1) /
           std::max(std::abs(z), std::numeric_limits<double>::epsilon());
  }
  /*!
   * Computes a point in the complex plane where \f$ |R(z)|-1=0 \f$
   * along a line starting from centerPoint \f$ c\f$ with angle \f$ \alpha\f$.
   * \f[
   * r(x) = c + xe^{i \alpha}
   * \f]
   *
   * The center point is by default \f$c= -1 + 0i\f$ but may be changed. It
   * should be a point \bf inside \bf the region of absolute stability.
   *
   * @param angle The angle \f$ \alpha\f$
   * @param centerPoint The centerpoint \f$ c\f$.
   * @return A tuple with the point and a boolean representing the status
   * (false=not found).
   */
  std::tuple<complex, bool>
  computeLimit(double const &angle, complex const &centerPoint = {-1., 0.})
  {
    auto [a, b] = bracket(angle, centerPoint);
    auto &  tol = options.tol;
    auto &  tola = options.tola;
    auto &  maxIt = options.maxIt;
    complex e = std::polar(1.0, angle); // e^{i angle}
    auto    f = [this, &e, &centerPoint](double const &x) {
      return std::abs(this->operator()(centerPoint + x * e)) - 1.0;
    };
    auto [x, status] = apsc::secant(f, a, b, tol, tola, maxIt);
    complex res = centerPoint + x * e;
    return std::make_tuple(res, status);
  };
  /*!
   * It finds two \f! x_1 \f$ and \f$ x_4 \f$ in the complex plane where \f$
   * |R(r(x)|-1\le 0 \f$ and \f$ |R(r(x))|-1 \ge 0 \f$, respectively. It
   * operated along a line starting from centerPoint \f$ c\f$ with angle \f$
   * \alpha\f$. \f[ r(x) = c + xe^{i \alpha} \f]
   *
   * The center point is by default \f$c= -1 + 0i\f$ but may be changed. It
   * should be a point \bf inside \bf the region of absolute stability.
   *
   * @param angle The angle \f$ \lapha \f$
   * @param centerPoint The center point
   * @return A pair containing the two points.
   */
  std::pair<double, double>
  bracket(double angle, complex const &centerPoint = {-1., 0.})
  {
    constexpr double       start = 0.;
    complex                e = std::polar(1.0, angle); // e^{i angle}
    constexpr unsigned int nTry = 50;
    constexpr double       step = 0.1;
    double                 x1 = start;
    double                 x2 = start + step;
    auto                   f = [this, &e, &centerPoint](double const &x) {
      return std::abs(this->operator()(centerPoint + x * e)) - 1.0;
    };
    unsigned int iTry = 0;
    while(f(x1) * f(x2) > 0 && iTry < nTry)
      {
        x1 = x2;
        x2 += step;
      }
    return {x1, x2};
  }
  /*!
   * Computes a numSamples point at the border of the stability region.
   *
   * @param numSamples The number of points
   * @param centerPoint The center point used in the construction
   * @see{computeLimit}
   * @return A vector of conplex with the found points.
   */
  std::vector<complex>
  computeStabilityRegion(unsigned int   numSamples,
                         complex const &centerPoint = {-1., 0.})
  {
    constexpr double     pi = 3.141592653589793238462643383279;
    constexpr double     start = 0.;
    constexpr double     end = pi;
    std::vector<complex> result;
    result.reserve(numSamples);
    for(unsigned int i = 0; i < numSamples; ++i)
      {
        double angle = start + i * (end - start) / (numSamples - 1);
        auto [x, status] = computeLimit(angle, centerPoint);
        if(status == true)
          {
            result.emplace_back(x);
          }
        else
          {
            if(options.verbose)
              {
                std::cerr << "Point at angle " << angle << " Not found\n";
              }
          }
      }
    result.shrink_to_fit();
    return result;
  }
  /*!
   * A struct that defines the optional parameters for the tool to find the zero
   * of a scalar function. It is used in computeStabilityRegion and computeLimit
   */
  struct Options
  {
    //! Relative tolerance
    double tol = 1.e-4;
    //! Absolute tolerance
    double tola = 1.e-10;
    //! Max number of iterations
    unsigned int maxIt = 150;
    //! Verbose output for computeStabilityRegion.
    bool verbose = false;
  };
  /*!
   * Contains the optional parameters to find the zero of a scalar function
   */
  Options options;

private:
  Butcher my_B;
};

} // namespace apsc

#endif /* EXAMPLES_SRC_RKFSOLVER_COMPUTERFORRK_HPP_ */
