/*
 * NonLinearSystem.hpp
 *
 *  Created on: Nov 10, 2018
 *      Author: forma
 *  THIS IS STILL A WORKING VERSION. USE nonLinSys.[hc]pp!
 *
 */

#ifndef SRC_NONLINSYS_NONLINEARSYSTEM_HPP_
#define SRC_NONLINSYS_NONLINEARSYSTEM_HPP_
#include "Eigen/Dense"
#include <functional>
#include <array>
#include <stdexcept>

namespace NonLinearSystems
{
  //! Templated container for a non linear system \f$ f(x): R^N\to R^N\f$
  /*!
    Compared to NonLinSys it stores the functions in a
    std::array and it does not allow to dynamically add functions.
    If the actual system contains less than N user added functions the
    remaining functions are set to zero automatically. So the system does
    in fact always contains N functions.
    /tpar N dymension of the system
   */
  template <unsigned int N>
  class NonLinearSystem
  {
  public:
    using argumentType=Eigen::Matrix<double,N,1>;
    using returnType=Eigen::Matrix<double,N,1>;
    using funType=std::function<double (argumentType const &)>;
    //! Compute \f$ -F(x) \f$
    returnType residual(argumentType const & x) const;
    //! Compute \f$ F(x) \f$.
    returnType operator()(argumentType const & x) const;
    //! Add a function to the system.
    void addToSystem(funType const & f);
    //! Number of equations.
    unsigned int numEq()const (return N;)
    //! Returns the i-th function.
    funType & operator [](unsigned int i){return M_funs[i];}
    //! Returns the i-th function.
    funType  operator [](unsigned int i) const {return M_funs[i];}
    //! Returns \f$ ||f(x)|| \f$
    double norm(argumentType const & x) const;
  private:
    //! The functions
    std::array<N,funType> M_Funs;
    //! The actual number of equations. The last N-len equations are set to zero
    unsigned int len{0u};
  };

  template <unsigned int N>
  NonLinearSystem<N>::returnType
    NonLinearSystem<N>::operator()(argumentType const & x) const
  {
    returnType tmp;
    for (unsigned int j=0u;j<len;++j)
      tmp[j]=M_funs[j](x);
    for (unsigned int j=len;j<N;++j)tmp[j]=0.
    return tmp;
  }

  template <unsigned int N>
  NonLinearSystem<N>::returnType
  NonLinearSystem<N>::residual(argumentType const & x) const
  {
    return -(this->operator()(x));
  }

  template <unsigned int N>
  void
  NonLinearSystem<N>::addToSystem(funType const & f)
  {
    if (this->len < N )
      this->M_Funs[this->len++]=f;
    else
      throw std::runtime_error("Too many functions");
  }

  template <unsigned int N>
  double
  NonLinearSystem<N>::norm(argumentType const & x) const
  {
    return this->operator ()(x).norm();
  }
}




#endif /* SRC_NONLINSYS_NONLINEARSYSTEM_HPP_ */
