#ifndef __INTERPOLATION_HPP__
#define __INTERPOLATION_HPP__
#include <vector>
#include <type_traits>
#include <algorithm>

//! @file interpolation.hpp
/*!
  @brief some utilities for 1D interpolation
  @details It explains the use of functors as policies.
 */
namespace FunctionApproximation{
  
  //!Lagrange interpolation
  
  template<class T>
  class LagrangeInterpolator{
  private:
    std::vector<T> _vx;
    std::vector<T> _vy;
  public:
    //! Call operator.
    /*!
      @par vx Vector with the x-coordinate of the interpolation point.
      @par vy Vector with the y-coordinates, y=y(x) at the interpolation points.
      @nore vx must contain not repeated entries
      @todo add default constructor and getters
     */
    template <class V>
    LagrangeInterpolator(V&& vx, V&& vy):
    _vx{std::forward<V>(vx)},_vy{std::forward<V>(vy)}{}

    /*!
     *       @par x  Coordinate where we want to compute the interpolation.
     *       @return The interpolated value.
     *
     */
    T operator()(T const & x) const
    {
      // returns 0 if we have no points
      if (_vx.empty() or _vy.empty()) return T{0};
      std::size_t n = _vx.size() - 1;
      T y{0.0};
      for (std::size_t i = 0u; i <= n; i++)
        {
          T temp{1.0};
          for (std::size_t j = 0u; j <= n; j++)
            if (j != i) temp *= (x - _vx[j])/(_vx[i] - _vx[j]);
          y += temp*_vy[i];
        }
      return y;
    }
  };

  //! Policy for pieacewise linear interpolation.
  /*!
   *  Here we require that the abscissa used for building the interpolant
   *  be ordered from the smallest to the greatest. No check is made to verify it!
   */
  template<class T>
    class PiecewiseLinearInterpolator{
    private:
      std::vector<T> _vx;
      std::vector<T> _vy;
    public:
      //! Call operator.
      /*!
        @par vx Vector with the x-coordinate of the interpolation point.
        @par vy Vector with the y-coordinates, y=y(x) at the interpolation points.
        @note vx must contain ordered (from smallest to greatest) and not repeated entries
        @todo add default constructor and getters
       */
      template <class V>
      PiecewiseLinearInterpolator(V&& vx, V&& vy):
      _vx{std::forward<V>(vx)},_vy{std::forward<V>(vy)}{}

      /*!
       *       @par x  Coordinate where we want to compute the interpolation.
       *       @return The interpolated value.
       *
       */
      T operator()(T const & x) const
      {
        // returns 0 if we have no points
        if (_vx.empty() or _vy.empty()) return T{0};
        // return vy[0] if only one point
        auto n = _vx.size();
        if (n==1u) return _vy[0];
        // now I need to find where x lies!
        // It is important that _vx contains ordered entries
        // since we are using an algorithm that operates on sorted ranges (and is log2n)
        // Otherwise, you may use find, but then it is order n.
        auto found = lower_bound(_vx.cbegin(),_vx.cend(),x);
        auto right = std::distance(_vx.cbegin(),found);
        if (right >=n)// we have to extrapolate x > max _vx
          right=n-1;
        else if (right==0)// we have to extrapolate x<= min _vx
          right = 1;
        auto left  = right - 1;
        return ( (x- _vx[left])*_vy[right]+(_vx[right]-x)*_vy[left] )/(_vx[right]-_vx[left]);
      }
    };
  //! Lagrange interpolation with Newton formula
  /*! 
    Newton formula is very convenient if one has to compute
    the value at many interpolation points.
    You need to precompute the coefficients of the Newton formula for
    polynomial interpolation. This is why this functor
    has no default constructor.
  */
  template <class T>
  class NewtonInterpolator
  {
  private:
    std::vector<T> coeff;
    std::vector<T> _vx;
  public:
    //! Constructor which precomputes the coefficients
    /*!
      @par vx Vector with the x-coordinate of the interpolation point.
      @par vy Vector with the y-coordinates, y=y(x) at the interpolation points.
    */
    template <class V>
    NewtonInterpolator(V&& vx, const std::vector<T>& vy):_vx{std::forward<V>(vx)}
    {
      this->computeNewtonCoef(this->_vx,vy);
	}
    //! Computes coefficients.
    /*! 
      We keep it public because one may want to recompute coefficients.
      @par vx Vector with the x-coordinate of the interpolation point.
      @par vy Vector with the y-coordinates, y=y(x) at the interpolation points.
    */     
    void computeNewtonCoef(const std::vector<T>& vx, const std::vector<T>& vy)
    {
      if (_vx.empty()) return;
      std::size_t n = vx.size() - 1;
      // find coefficients in Newton's form
      coeff=vy;
      for (std::size_t j = 1u; j <= n; j++)
	for (std::size_t i = n; i >= j; i--)
	  coeff[i] = (coeff[i] - coeff[i-1])/(_vx[i] - _vx[i-j]);
    }
    
    //! Call operator.
    /*!
      @par x  Coordinate where we want to compute the interpolation.
      @return The interpolated value.
    */
    T operator()(T const & x) const {
      if (_vx.empty() or this->coeff.empty()) return T{0};
      //std::size_t n = _vx.size() - 1;
      // evaluate interpolation polynomial at x
      // A use ot reverse iterators
      auto vxi = _vx.crbegin();
      T u{0.0};
      for (auto ci=coeff.crbegin();ci!=coeff.crend();++ci,++vxi)
        u=(x-*vxi)*u + *ci;
      /*T u = coeff[n];
      if (n>0)
        {
          std::size_t i = n - 1;
          for (std::size_t i = n - 1; i >= 0u; i--)
            u = coeff[i] + (x - this->_vx[i])*u;
        }*/
      return u;
    }
  };
  


  
  //! Computes interpolation at several points.
  /*
    @par xcoord  Coordinates where we want to compute the interpolation.
    @par interpolator The policy implementing the interpolation
    @return The computed interpolated values
    @note default value for function template parameters is a C++11 extension.
  */
  template
  <class T, class Interpolator=LagrangeInterpolator<T> >
  std::vector<T> interp1D(const std::vector<T>& xcoord, Interpolator  const & interpolator)
  {
    std::vector<T> v;
    v.reserve(xcoord.size());
    for (auto const & i : xcoord) v.emplace_back(interpolator(i));
    return v;
  }
}
#endif
