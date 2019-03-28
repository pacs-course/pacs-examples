/*
 * RKF.hpp
 *
 *  Created on: Feb 4, 2019
 *      Author: forma
 */

#ifndef SRC_RK45_RKF_HPP_
#define SRC_RK45_RKF_HPP_
#include <vector>
#include <functional>
#include <limits>
#include <cmath>
#include <iosfwd>
#include "ButcherRKF.hpp" // not strictly needed here but it simplifies things
namespace ODE
{
  //! Struct holding the result for the solver
  struct RKFResult
  {
    //! Time steps
    std::vector<double> time;
    //!values
    std::vector<double> y;
    //! estimated error
    double estimatedError{0.0};
    //! Failure
    bool failed{false};
    //! Number of time step expansions
    int expansions{0};
    //! Number of time step contractions
    int contractions{0};
  };

  template<class B>
  class RKF
  {
  public:
    //! The forcing term \f$f(t,y)\f$ is wrapped in a std::function
    using Function= std::function<double (double const & t, double const & y)>;
    //! Constructor passing butcher table and forcing function
    RKF(B const & bt, Function const & f):M_f(f),ButcherTable(bt){};
    //! Default constructor
    RKF()=default;
    //! Set the forcing function
    void set_function(Function const & f){M_f=f;}
    //! Set the Butcher Array
    void set_ButcherArray(B const & bt){ButcherTable=bt;}

    /*!
     * @param T0 initial time
     * @param T  final time
     * @param y0 initial condition
     * @param hInit initial time step
     * @param tol desired global error max. norm
     * @param maxstep Safeguard to avoid too many steps (default 2000)
     */
    RKFResult operator()(double const & T0, double const & T, double const & y0, double const & hInit, double const & tol, int maxStep=2000) const;
  private:
    Function M_f;
    B ButcherTable;
    /*! Function for a single step. It is private since is used only internally.
     *
     * @param tstart start time
     * @param y0 value at tstart
     * @param h time step
     * @return y values computed with the two rules
     */
    std::pair<double,double> RKFstep(const double & tstart, const double & y0, const double & h) const;
  };
} //end namespace

template<class B>
  ODE::RKFResult
  ODE::RKF<B>::operator () (const double& T0, const double& T,
                            const double & y0, const double& hInit, const double& tol,
                            int maxSteps) const
  {
  RKFResult res;
  // Useful alias to simplify typing
  std::vector<double> & time=res.time;
  std::vector<double> & y=res.y;
  auto & expansions=res.expansions;
  auto & contractions=res.contractions;
  auto & estimatedError=res.estimatedError;
  estimatedError=0.0; // set initial error to zero
  auto & failed=res.failed;
  failed=false; // set failed to false
  //  reserve some space according to data
  int expectedSteps=std::min(std::max(1,1+static_cast<int>((T-T0)/hInit)),maxSteps);
  time.reserve(expectedSteps);
  y.reserve(expectedSteps);
  // push initial step
  time.push_back(T0);
  y.push_back(y0);
  // to check if a step has been rejected
  bool rejected(false);
  // fraction of time step if error greater/lower than tolerance
  double constexpr reductionFactor=0.90;
  // I expand at most doubling h, for safety
  double constexpr expansionFactor=2.;
  // Now I need a factor to specify when I can enlarge the time step
  // to avoid reducing and expanding the time step repeatedly
  // I need to take into account the order of the scheme if >2
  double factor = 1./(ButcherTable.order-1);
  // Iteration counter
  unsigned int iter=0;
  // I want to check that the time step does not go ridiculosly small
  double hmin=100*std::numeric_limits<double>::epsilon();
  double h=std::max(hInit,hmin);
  double t=T0;
  double ycurr=y0;
  double delta=T-T0;
  while (t<T && iter<=maxSteps)
    {
      ++iter;
      double ylow;
      double yhigh;
      double errorPerTimeStep=tol*h/delta;
      // Check if new time step will cross the final time step
      if(t+h>T)h=T-t;
      std::tie(ylow,yhigh)=RKFstep(t,ycurr,h);
      double currentError=std::abs(ylow-yhigh);
      double mu=std::pow(errorPerTimeStep/currentError,factor);
      if(currentError<=errorPerTimeStep)
        {
          //fine set new point!
          t=t+h;
          time.push_back(t);
          y.push_back(yhigh);
          ycurr=yhigh;
          estimatedError+=currentError;
          // Expand next step if error very small, step not previously rejected and I am not at the end
          if((mu>=expansionFactor) && !rejected && (t<T))
            {
              h*=expansionFactor;
              ++expansions;
            }
          rejected=false;
        }
      else
        {
          rejected=true;
          h*=mu*reductionFactor;// a little more to be sure
          ++contractions;
          h= h<=hmin? hmin: h;
        }
    }
  if(iter>maxSteps) failed=true;
  return res;
  }

template<class B>
  std::pair<double, double>
  ODE::RKF<B>::RKFstep (const double& tstart, const double & y0, const double& h) const
  {
    auto constexpr Nstages=B::Nstages();
    std::array<double,Nstages> K;
    // I use references to simplify typing
    typename B::Atable const & A=ButcherTable.A;
    std::array<double,Nstages> const & c{ButcherTable.c};
    std::array<double,Nstages> const & b1{ButcherTable.b1};
    std::array<double,Nstages> const & b2{ButcherTable.b2};
    //! The first step is always an Euler step
    K[0]=M_f(tstart,y0)*h;
    for (unsigned int i=1;i<Nstages;++i)
      {
        double time=tstart+c[i]*h;
        double value=y0;
        for (unsigned int j=0;j<i;++j)value+=A[i][j]*K[j];
        K[i]=M_f(time,value)*h;
      }
    double v1=y0;
    double v2=y0;
    for (unsigned int i=0; i<Nstages;++i)
      {
        v1+=K[i]*b1[i];
        v2+=K[i]*b2[i];
      }
    return std::make_pair(v1,v2);
  }

//! streaming operator to dump the results in gnuplot format
std::ostream & operator << (std::ostream & out,ODE::RKFResult const & res);


#endif /* SRC_RK45_RKF_HPP_ */
