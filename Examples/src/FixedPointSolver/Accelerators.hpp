/*
 * Accelerators.hpp
 *
 *  Created on: Jan 25, 2020
 *      Author: forma
 */

#ifndef SRC_NONLINSYSSOLVER_ACCELERATORS_HPP_
#define SRC_NONLINSYSSOLVER_ACCELERATORS_HPP_
#include<memory>
#include <functional>
#include <cmath>
#include "FixedPointTraits.hpp"
//! \file Accelerators.hpp Different accelerators for fixed point iteration
namespace apsc
{
  //! An enum that stores the different options made available
  enum FPAcceleratorId {NoAcceleration,ASecant};

  //! The simple iterator
  /*!
   * This class is very thin since it only defines a call operator that
   * given a new iterate returns the application of the iteration function. This class is called
   * noAcceleration, since it just implements the simple fixed point iteration step
   *
   * \tparam IteratorFunction. The iterator function. Note that at this level the iterator function
   * is used only as a trait to extract the type used for the arguments. It must conform to a std::function!
   *
   */
  template<FixedPointArgumentType ARG>
  class NoAccelerator
  {
  public:
    using ArgumentType=typename FixedPointTraits<ARG>::ArgumentType;
    using IterationFunction=typename FixedPointTraits<ARG>::IterationFunction;
    NoAccelerator(IterationFunction const & I):M_I{I}{};//!< Does nothing
    //! Call operator that returns the accelerated iterate
    ArgumentType operator()(ArgumentType const &x){return M_I(x);}
    /*!! A different call operator, which takes two arguments,
     *	since some accelaration techniques are based on two sequences.
     */
    ArgumentType operator()(ArgumentType const & x,ArgumentType const &){return M_I(x);}
    //! Puts the accelerator to the original setting
    void reset(){};
    //! Returns the identifier of the accelerator. Useful to implement different choices run-rime
    constexpr FPAcceleratorId getId(){return NoAcceleration;}
    //! The identifier is an attribute of the class. It may be tested on the type.
    static constexpr FPAcceleratorId id=NoAcceleration;
  private:
    const IterationFunction & M_I;
  };

  namespace AccelInternals
  {
    //! Helper function to express dot product
    template <class T>
    double dot (std::size_t len,T const & a, T const & b)
    {
      double res{0.0};
      for (std::size_t i=0;i<len;++i)
        res+=a[i]*b[i];
      return res;
    }
    //! Helper function to compute squared norm
    template <class T>
    double squaredNorm(std::size_t len,T const & a)
    {
      return dot(len,a,a);
    }
    //! Helper function to add vectors
    //! @todo It may be secialized for Eigen vectors
    template <class T>
    void plus(std::size_t len,T const & a, T const & b, T & res)
    {
      for (std::size_t i=0;i<len;++i)
        res[i]=a[i]+b[i];
    }
    //! Helper function to subtract vectors
    //! @todo It may be specialized for Eigen vectors
    template <class T>
    void minus(std::size_t len,T const & a, T const & b, T & res)
    {
      for (std::size_t i=0;i<len;++i)
        res[i]=a[i]-b[i];
    }
    //! Helper function to scale a vector
    template <class T>
    void scale(std::size_t len, double a, T const & b, T & res)
    {
      for (std::size_t i=0;i<len;++i)
        res[i]=a*b[i];
    }
    //! Helper function to compute squared distance among vectores
    template <class T>
    double squareDistance(std::size_t len,T const & a, T const & b)
    {
      double res{0.0};
      for (std::size_t i=0;i<=len;++i)
        res+=(a[i]-b[i])*(a[i]-b[i]);
      return res;

    }
  }// end internals

  //! Alternate secant method
  /*!
   * This is a two-level acceleration method equivalent to a two level Anderson method
   * \f[
   * x_{n+1}=\phi(x_n)-\frac{(\Delta x_n -\Delta x_{n-1})\cdot\Delta x_n}{||\Delta x_n -\Delta x_{n-1}||^2}(\phi(x_n)-\phi(x_{n-1})
   * \f]
   *
   * V. Eyert, A Comparative Study on Methods for Convergence Acceleration of Iterative Vector Sequences, Journal of Computational Physics 124 (1996) 271–285.
   * H. Fang, Y. Saad, Two classes of multisecant methods for nonlinear accel- eration, Numerical Linear Algebra with Applications 16 (2009) 197–221.
   *
   * \tparam IteratorFunction. The iterator function. Note that at this level the iterator function
   * is used only as a trait to extract the type used for the arguments. No other requirements are made
   *
   */
  template<FixedPointArgumentType ARG>
  class ASecantAccelerator
  {
  public:
    using ArgumentType=typename FixedPointTraits<ARG>::ArgumentType;
    using IterationFunction=typename FixedPointTraits<ARG>::IterationFunction;
    ASecantAccelerator(IterationFunction const & I):M_I{I}{};//!< Does nothing
    //! Call operator that returns the accelerated iterate
    ArgumentType operator()(ArgumentType const &x);
    /*!! A different call operator, which takes two arguments,
     *	since some accelaration techniques are based on two sequences.
     */
    ArgumentType operator()(ArgumentType const & x,ArgumentType const &){return this->operator()(x);}
    //! Put back the accelerator to initial setting
    void reset()
    {
      length=0;
      firstTime=true;
    }
    //! Returns the identifier of the accelerator. Useful to implement different choices run-rime
    constexpr FPAcceleratorId getId(){return id;}
    //! The identifier is an attribute of the class. It may be tested on the type.
    static constexpr FPAcceleratorId id=ASecant;
  private:
    const IterationFunction & M_I;
    ArgumentType deltaXOld;
    ArgumentType xOld;
    ArgumentType phiOld;
    size_t length{0};
    ArgumentType xNew;
    ArgumentType deltaXNew;
    bool firstTime=true;
  };
  
  template <FixedPointArgumentType ARG>
  typename ASecantAccelerator<ARG>::ArgumentType  ASecantAccelerator<ARG>::operator()(ArgumentType const &x)
  {
    if (firstTime)
      {
        // set initial values
        this->length=x.size();
        this->xNew=this->M_I(x);
        this->phiOld=xNew;
        deltaXOld.resize(length);
        deltaXNew.resize(length);
        // compute \Delta x_n = phi(x_n)-x_n
        AccelInternals::minus(length,xNew,x,deltaXOld);
        firstTime=false;
      }
    else
      {
        this->xNew=this->M_I(x);
        // compute \Delta x_n = phi(x_n)-x_n
        AccelInternals::minus(length,xNew,x,deltaXNew);
        ArgumentType tmp(length);
        // \Delta x_n - \Delta x_{n-1}
        AccelInternals::minus(length,deltaXNew,deltaXOld,tmp);
        // Get ||\Delta x_n - \Delta x_{n-1}||^2
        double norm2=AccelInternals::squaredNorm(length,tmp);
        //to do: check norm2
        norm2 =1.0/norm2;
        ArgumentType tmp2(length);
        //! Scaling:  \Delta x_n - \Delta x_{n-1}/||\Delta x_n - \Delta x_{n-1}||^2
        AccelInternals::scale(length,norm2,tmp,tmp2);
        //! Do the dot product with \Delta x_n
        double factor=AccelInternals::dot(length,tmp2,deltaXNew);
        // Save phi(X_n) for later use
        tmp2=xNew;
        // scale (phi(x_n)-\phi(x_{n-1}) factor and subtract
        // 
        for (std::size_t i=0;i<length;++i)
          xNew[i]-=factor*(xNew[i]-phiOld[i]);
        phiOld=tmp2;
        deltaXOld=deltaXNew;
      }
    return xNew;
  }
}// end namespace apsc



#endif /* SRC_NONLINSYSSOLVER_ACCELERATORS_HPP_ */
