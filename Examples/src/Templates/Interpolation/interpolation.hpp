#ifndef __INTERPOLATION_HPP__
#define __INTERPOLATION_HPP__
#include <vector>   

//! @file interpolation.hpp
/*!
  @brief some utilities for 1D interpolation
  @details It explains the use of functors as policies.
 */
namespace FunctionApproximation{
  
  //!Lagrange interpolation
  
  template<class T>
  class LagrangeInterpolator{
  public:
    //! Call operator.
    /*!
      @par vx Vector with the x-coordinate of the interpolation point.
      @par vy Vector with the y-coordinates, y=y(x) at the interpolation points.
      @par x  Coordinate where we want to compute the interpolation.
      @return The interpolated value.
     */
    T operator()(
		 const std::vector<T>& vx, const std::vector<T>& vy, 
		 T const & x) {
      int n = vx.size() - 1;
      T y = 0;
      for (int i = 0; i <= n; i++) {
	T temp = 1;
	for (int j = 0; j <= n; j++) 
	  if (j != i) temp *= (x - vx[j])/(vx[i] - vx[j]);
	y += temp*vy[i];
      }
      return y;
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
  public:
    //! Constructor which precomputes the coefficients
    /*!
      @par vx Vector with the x-coordinate of the interpolation point.
      @par vy Vector with the y-coordinates, y=y(x) at the interpolation points.
    */
    NewtonInterpolator(const std::vector<T>& vx, const std::vector<T>& vy){
      this->computeNewtonCoef(vx,vy);
	}
    //! No precomputation of the coefficients
    NewtonInterpolator()=delete; //C++11 extension
    //! Computes coefficients.
    /*! 
      We keep it public because one may want to recompute coefficients.
      @par vx Vector with the x-coordinate of the interpolation point.
      @par vy Vector with the y-coordinates, y=y(x) at the interpolation points.
    */     
    void computeNewtonCoef(const std::vector<T>& vx, const std::vector<T>& vy)
    {
      unsigned int n = vx.size() - 1;
      // find coefficients in Newton's form
      coeff=vy;
      for (unsigned int j = 1; j <= n; j++) 
	for (unsigned int i = n; i >= j; i--) 
	  coeff[i] = (coeff[i] - coeff[i-1])/(vx[i] - vx[i-j]);
    }
    
    //! Call operator.
    /*!
      @par vx Vector with the x-coordinate of the interpolation point.
      @par vy Vector with the y-coordinates, y=y(x) at the interpolation points.
      @par x  Coordinate where we want to compute the interpolation.
      @return The interpolated value.
    */
    T operator()(const std::vector<T>& vx, 
		 const std::vector<T>& vy, T const & x){
      int n = vx.size() - 1;
      // evaluate interpolation polynomial at x
      T u = coeff[n];
      for (int i = n - 1; i >= 0; i--) 
	u = coeff[i] + (x - vx[i])*u;
      return u;
    }
  };
  
  
  //! Computes interpolation at several points.
  /*
    @par vx Vector with the x-coordinate of the interpolation point.
    @par vy Vector with the y-coordinates, y=y(x) at the interpolation points.
    @par xcoord  Coordinates where we want to compute the interpolation.
    @par interpolator The policy implementing the interpolation
    @return The computed interpolated values
    @note default value for function template parameters is a C++11 extension.
  */
  template
  <class T, class Interpolator=LagrangeInterpolator<T> >
  std::vector<T> interp1D(const std::vector<T>& vx,const std::vector<T>& vy,
			  const std::vector<T>& xcoord, 
			  Interpolator 
			  interpolator=Interpolator()){
    
    std::vector<T> v;
    v.reserve(xcoord.size());
    for (auto const & i : xcoord) v.emplace_back(interpolator(vx,vy,i));
    return v;
  }
}
#endif
