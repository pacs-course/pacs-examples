#ifndef HH_FIXEDPOINT_HH_
#define HH_FIXEDPOINT_HH_
#include  "nonLinSys.hpp"
namespace NonLinearSystems{
  //! Struct that holds the option for Fixed Point iterations.
  /*!
    It provides the parameter to guide the iterations.
   */
  struct FixedPointOptions{
    //! @brief Tolerance on residual.
    /*! @detail The iteration stops if \f$||F(x)||<minRes\f$.*/
    double minRes;
    //! MAx. number of iterations.
    unsigned int maxIter;
    //! Relaxation parameter
    double alpha;
  };

  //! Status returned by the Fixed Point Iteration.
  struct FixedPointStatus{
    //! Last residual.
    double residual;
    //! Last iteration.
    unsigned int iterations;
    //! @brief Converged flag.
    /*! @detail true if two successive iterations smaller
     than given tolerance*/
    bool converged;
  };
  
  //! Function implementing a fixed point method for zero of non-linear systems
  /*!

    It solves
    \f[
    x^{k+1}=x^{k}-\alpha r(x)
    \f]
    where \f$ r(x) \f$ is the residual;
 
    The iteration stops under one of the three conditions:
    - number of iterations exceeded
    - residual less or equal opt.minRes
    - norm of two successive iteration below opt.tolerance
    
    @param fSys The non linear system.
    @param x    In input the starting point. In output the found point.
    @param opt  The given options. 
  */
  FixedPointStatus fixedPoint(NonLinSys const & fSys,
			  argumentType & x,
			  const FixedPointOptions opt={1.e-08,100,1.0});
}
#endif
