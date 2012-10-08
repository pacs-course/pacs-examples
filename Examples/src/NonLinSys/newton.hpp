#ifndef HH_NEWTON_HH_
#define HH_NEWTON_HH_
#include  "nonLinSys.hpp"
#include  "jacob.hpp"
namespace NonLinearSystems{
  //! Struct that holds the option for Newton iterations.
  /*!
    It provides the parameter to guide the iterations.
   */
  struct NewtonOptions{
    //! @brief Tolerance for two successive iterations.
    /*! @detail The iteration stops if \f$||x_{new}-x_{old}||<tolerance\f$. */
    double tolerance;
    //! @brief Tolerance on residual.
    /*! @detail The iteration stops if \f$||F(x)||<minRes\f$.*/
    double minRes;
    //! MAx. number of iterations.
    unsigned int maxIter;
  };

  //! Status returned by the function Newton.
  struct NewtonStatus{
    //! Last residual.
    double residual;
    //! Last iteration.
    unsigned int iterations;
    //! @brief Converged flag.
    /*! @detail true if two successive iterations smaller
     than given tolerance*/
    bool converged;
  };
  
  //! Function implementing Newton method for zero of non-linear systems.
  /*!
    The iteration stops under one of the three conditions:
    - number of iterations exceeded
    - residual less or equal opt.minRes
    - norm of two successive iteration below opt.tolerance
    
    @param fSys The non linear system.
    @param jac  The Jacobian.
    @param x    In input the starting point. In output the found point.
    @param opt  The given options. 
  */
  NewtonStatus Newton(NonLinSys const & fSys,
		      Jacobian const & jac,
		      argumentType & x,
		      const NewtonOptions opt={1.e-08,1.e-09,100});
}
#endif
