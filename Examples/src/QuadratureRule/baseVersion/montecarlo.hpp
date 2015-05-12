#ifndef MONTECARLO_HPP
#define MONTECARLO_HPP
#include "QuadratureRule.hpp"
namespace NumericalIntegration{
  
  //! Implements montecarlo quadrature
  /*
    This quadrature rule computes
    \f$ \int_a^b f \simeq \frac{1}{N}\sum_{i=0}^{N-1} f(x_i)\f$

    You may indicate the desired error on the interval
    (default 1.e-02) and the max number of sampling.
    
   */
  class MonteCarlo: public QuadratureRule
  {
  public:
    MonteCarlo();
    virtual std::unique_ptr<QuadratureRule> clone()const override;
    virtual double apply(FunPoint const &, double const & a,
             double const & b) const override;
    //! Sets desired maximal error
    void setError(double e);
    //! Sets maximal number of samples;
    void setMiter(unsigned int m);
    //! Returns the last computed error estimate;
    double lastError()const{return M_lastError;};
    //! Returns the error estimate;
    double cumulatedError()const {return M_cumulatedError;};
    //! Warns if iterations exceeded
    bool iterationsExceeded() const {return M_iterationsExceeded;}
    //! Resets the error accumulators.
    void resetError();
    virtual ~MonteCarlo(){}
  private:
    //! Max iterations
    unsigned int max_iter;
    //! Desired error;
    double error_level;
    //! The last error
    /*!
    Mutable because I want to be able to change it also
    on constant Montecarlo objects
    */
    mutable double M_lastError;
    //! The cumulated error
    mutable double M_cumulatedError;
    //! Iterations exceeded
    mutable bool M_iterationsExceeded;
    //! Default mx number of iterations
    static constexpr unsigned int max_iter_def=10000;
    //! Default error level;
    /*
      definition in the cpp file
     */
    static constexpr double error_level_def=1.0e-2;
  };
  //  constexpr double MonteCarlo::error_level_def(1.0e-2);

}



#endif
