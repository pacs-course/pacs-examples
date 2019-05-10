#ifndef HH_FIXEDPOINT_HH_
#define HH_FIXEDPOINT_HH_
#include  "nonLinSys.hpp"
#include <utility>
#include <type_traits>
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
			  NonLinSys::argumentType & x,
			  const FixedPointOptions opt={1.e-08,100,1.0});


 /*
  * Another implementation, maybe more elegant
  */

  /*! It implements the fixed point operator g=x-F(x)
   * @tparam NLS A non linear system
   *  @note this is only an experiment. I have commented it away
   */
  /*
  template <class NLS>
  class FixedPoint
  {
  public:
    static_assert(std::is_convertible<typename NLS::argumentType,typename NLS::returnType>::value,"argument and return type must be convertible");

    template<typename N>
    FixedPoint(N&& nls=NLS()):M_nls(std::forward<N>(nls)){}; //!<constructor with possible move

    FixedPoint &
    operator =(FixedPoint<NLS> const &)=default;

    FixedPoint &
    operator =(FixedPoint<NLS> &&)=default;

    template <class N>
    void setNonLinearSystem(N&& nls) //!< move if possible
    {
      this->M_nls=std::forward<N>(nls);
    }

    typename NLS::returnType
    operator()(typename NLS::argumentType const & x) const
    {
      return x - this->M_nls(x);
    }

  private:
    NLS M_nls;
  };
*/
}
#endif
