/*
 * interpolationPolicies.hpp
 *
 *  Created on: Mar 27, 2021
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_TEMPLATES_INTERPOLATION_INTERPOLATIONPOLICIES_HPP_
#define EXAMPLES_SRC_TEMPLATES_INTERPOLATION_INTERPOLATIONPOLICIES_HPP_
#include <vector>
#include <type_traits>
#include <algorithm>

namespace apsc::interpolationPolicy{

  /*!
   * Policy for Lagrange interpolation.
   * Given two vectors with n elements it builds the interpolation polinomial of degree n-1
   */
  class LagrangeInterpolator{
  private:
    std::vector<double> vx_;
    std::vector<double> vy_;
  public:
    /*!
     * Constructor. Takes two vectors
      @tparam V any type that can be converted to a std::vector<double>
      @param vx Vector with the x-coordinate of the interpolation point.
      @param vy Vector with the y-coordinates, y=y(x) at the interpolation points.
      @pre vx must contain not repeated entries
      @pre vx and vy must have the same size. Size of vx -1 is the polinomial degree
     */
    template <class V>
    LagrangeInterpolator(V&& vx, V&& vy):
    vx_{std::forward<V>(vx)},vy_{std::forward<V>(vy)}{}

     LagrangeInterpolator()=default;
     /*!
      *
      * @tparam V any type that can be converted to a std::vector<double>
      * @param vx Vector with the x-coordinate of the interpolation point.
      * @param vy Vector with the y-coordinates, y=y(x) at the interpolation points.
      */
     template <class V>
     void setNodes(V&& vx, V&& vy)
     {
        vx_=std::forward<V>(vx);
        vy_=std::forward<V>(vy);
     }

    /*!
     * @param x  Coordinate where we want to compute the interpolation.
     * @return The interpolated value.
     */
    double operator()(double const & x) const
    {
      // returns 0 if we have no points
      if (vx_.empty() or vy_.empty()) return 0.0;
      std::size_t n = vx_.size() - 1;
      double y{0.0};
      for (std::size_t i = 0u; i <= n; i++)
        {
          double temp{1.0};
          for (std::size_t j = 0u; j <= n; j++)
            if (j != i) temp *= (x - vx_[j])/(vx_[i] - vx_[j]);
          y += temp*vy_[i];
        }
      return y;
    }
  };

  //! Policy for pieacewise linear interpolation.
  /*!
   *  Here we require that the abscissa used for building the interpolant
   *  be ordered from the smallest to the greatest. No check is made to verify it!
   */
    class PiecewiseLinearInterpolator{
    private:
      std::vector<double> vx_;
      std::vector<double> vy_;
    public:

      /*!
        * Constructor. Takes two vectors
         @tparam V any type that can be converted to a std::vector<double>
         @param vx Vector with the x-coordinate of the interpolation point.
         @param vy Vector with the y-coordinates, y=y(x) at the interpolation points.
         @pre vx must contain not repeated entries
         @pre vx and vy must have the same size. Size of vx -1 is the polinomial degree
        */
      template <class V>
      PiecewiseLinearInterpolator(V&& vx, V&& vy):
      vx_{std::forward<V>(vx)},vy_{std::forward<V>(vy)}{}

      PiecewiseLinearInterpolator()=default;
      /*!
        *
        * @tparam V any type that can be converted to a std::vector<double>
        * @param vx Vector with the x-coordinate of the interpolation point.
        * @param vy Vector with the y-coordinates, y=y(x) at the interpolation points.
        */
       template <class V>
       void setNodes(V&& vx, V&& vy)
       {
          vx_=std::forward<V>(vx);
          vy_=std::forward<V>(vy);
       }

      /*!
       * @param x  Coordinate where we want to compute the interpolation.
       * @return The interpolated value.
       *
       */
      double operator()(double const & x) const
      {
        // returns 0 if we have no points
        if (vx_.empty() or vy_.empty()) return 0.0;
        // return vy[0] if only one point
        int n = vx_.size();
        if (n==1u) return vy_[0];
        // now I need to find where x lies!
        // It is important that _vx contains ordered entries
        // since we are using an algorithm that operates on sorted ranges (and is log2n)
        // Otherwise, you may use find, but then it is order n.
        auto found = lower_bound(vx_.cbegin(),vx_.cend(),x);
        auto right = std::distance(vx_.cbegin(),found);
        if (right >=n)// we have to extrapolate x > max _vx
          right=n-1;
        else if (right==0)// we have to extrapolate x<= min _vx
          right = 1;
        auto left  = right - 1;
        return ( (x- vx_[left])*vy_[right]+(vx_[right]-x)*vy_[left] )/(vx_[right]-vx_[left]);
      }
    };
  //! Lagrange interpolation with Newton formula
  /*!
    Newton formula is very convenient if one has to compute
    the value at many interpolation points.
    You need to precompute the coefficients of the Newton formula for
    polynomial interpolation.
  */
  class NewtonInterpolator
  {
  private:
    std::vector<double> coeff;
    std::vector<double> vx_;
  public:
    //! Constructor which precomputes the coefficients
    /*!
      @tparam V any type that can be converted to a std::vector<double>
      @param vx Vector with the x-coordinate of the interpolation point.
      @param vy Vector with the y-coordinates, y=y(x) at the interpolation points.
    */
    template <class V>
    NewtonInterpolator(V&& vx, V&& vy):coeff{std::forward<V>(vy)},vx_{std::forward<V>(vx)}
    {
      computeNewtonCoeff();
     }
    NewtonInterpolator()=default;
    /*!
      *
      * @tparam V any type that can be converted to a std::vector<double>
      * @param vx Vector with the x-coordinate of the interpolation point.
      * @param vy Vector with the y-coordinates, y=y(x) at the interpolation points.
      */
     template <class V>
     void setNodes(V&& vx, V&& vy)
     {
        vx_=std::forward<V>(vx);
        coeff=std::forward<V>(vy);
        computeNewtonCoeff();
     }

    //! Call operator.
    /*!
      @par x  Coordinate where we want to compute the interpolation.
      @return The interpolated value.
    */
    double operator()(double const & x) const {
      if (vx_.empty() or coeff.empty()) return 0.0;
      //std::size_t n = vx_.size() - 1;
      // evaluate interpolation polynomial at x
      // A use ot reverse iterators
      auto vxi = vx_.crbegin();
      double u{0.0};
      for (auto ci=coeff.crbegin();ci!=coeff.crend();++ci,++vxi)
        u=(x-*vxi)*u + *ci;
      return u;
    }
  private:
    //! Computes coefficients.
    /*!
      We keep it public because one may want to recompute coefficients.
      @par vx Vector with the x-coordinate of the interpolation point.
      @par vy Vector with the y-coordinates, y=y(x) at the interpolation points.
    */
    void computeNewtonCoeff()
    {
      if (vx_.empty()) return;
      std::size_t n = vx_.size() - 1;
      // find coefficients in Newton's form
      for (std::size_t j = 1u; j <= n; j++)
        for (std::size_t i = n; i >= j; i--)
          coeff[i] = (coeff[i] - coeff[i-1])/(vx_[i] - vx_[i-j]);
    }

  };

}



#endif /* EXAMPLES_SRC_TEMPLATES_INTERPOLATION_INTERPOLATIONPOLICIES_HPP_ */
