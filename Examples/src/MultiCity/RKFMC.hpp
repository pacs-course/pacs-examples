/*
 * RKF.hpp
 *
 *  Created on: Feb 4, 2019
 *      Author: forma
 */

#ifndef SRC_RK45_RKFMC_HPP_
#define SRC_RK45_RKFMC_HPP_
#include <vector>
#include <functional>
#include <limits>
#include <cmath>
#include <iostream>
#include "ButcherRKF.hpp" // not strictly needed here but it simplifies things
#include "RKFTraits.hpp"
namespace apsc
{
  namespace multicity
{
  //! Struct holding the result for the solver
  template <class VariableType>
  struct RKFResult
  {
    //! Time steps
    std::vector<double> time;
    //!values
    std::vector<VariableType> y;
    //! estimated error
    double estimatedError{0.0};
    //! Failure
    bool failed{false};
    //! Number of time step expansions
    int expansions{0};
    //! Number of time step contractions
    int contractions{0};
    //! return last value
    std::tuple<double, VariableType>
    lastValues()const {return std::make_tuple(time.back(),y.back());}
  };

  template<class B,class Model>
  class RKFMC
  {
  public:
    using VariableType=typename Model::VariableType;
    using Function= typename Model::ForcingTermType;
    //! Constructor passing butcher table and forcing function
    RKFMC(B const & bt, Function const & f):M_f(f),ButcherTable(bt){};
    //! Default constructor
    RKFMC()=default;
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
    RKFResult<VariableType>
    operator()(double const & T0, double const & T, VariableType const & y0,
               double const & hInit, double const & tol, unsigned int maxStep=2000) const;
  private:
    Function M_f;
    B ButcherTable;
    /*! Function for a single step. It is private since is used only internally.
     *
     * @note
     * I have used trailing return type syntax, to be consistent with the definition given
     * outside this class template definition. But here it is not necessary. I could have written
     * @code
     * std::pair<VariableType,VariableType> RKFstep(const double & tstart, const VariableType & y0, const double & h)
     * @endcode
     * since in this context VariableType is well defined
     *
     * @param tstart start time
     * @param y0 value at tstart
     * @param h time step
     * @return values computed with the two rules
     */
   auto RKFstep(const double & tstart, const VariableType & y0, const double & h) const ->
       std::pair<VariableType,VariableType>;
  };

  //! streaming operators to dump the results in gnuplot format
  //!  For simplicity I inline them so I have everything in this header file
  template<class VariableType>
  inline std::ostream & operator << (std::ostream & out, RKFResult<VariableType> const & res);

//   ***********************************************
//   ******    IMPLEMENTATIONS OF TEMPLATE FUNCTIONS
//   ***********************************************

  template<class B, class Model>
  auto
  RKFMC<B,Model>::operator () (const double& T0, const double& T,
                            const VariableType & y0, const double& hInit, const double& tol,
                            unsigned int maxSteps) const
  -> RKFResult<VariableType>
  {
  RKFResult<VariableType> res;
  // Useful alias to simplify typing
  std::vector<double> & time=res.time;
  std::vector<VariableType> & y=res.y;
  auto & expansions=res.expansions;
  auto & contractions=res.contractions;
  auto & estimatedError=res.estimatedError;
  estimatedError=0.0; // set initial error to zero
  auto & failed=res.failed;
  failed=false; // set failed to false
  //  reserve some space according to data
  unsigned int expectedSteps=std::min(std::max(1u,1u+static_cast<unsigned int>((T-T0)/hInit)),maxSteps);
  time.reserve(expectedSteps);
  y.reserve(expectedSteps);
  // push initial step
  time.push_back(T0);
  y.push_back(y0);
  // to check if a step has been rejected
  bool rejected(false);
  // fraction of time step if error greater than tolerance
  double constexpr reductionFactor=0.95;
  // I expand at most doubling h, for safety
  double constexpr expansionFactor=2.;
  // Now I need a factor to specify when I can enlarge the time step
  // to avoid reducing and expanding the time step repeatedly
  // I need to take into account the order of the scheme is >2
  double factor;
  if (ButcherTable.order <=2)
    factor=1.0;
  else
    factor= 1./(ButcherTable.order-1);
  // Iteration counter
  unsigned int iter=0;
  // I want to check that the time step does not go ridiculosly small
  double hmin=100*(T-T0)*std::numeric_limits<double>::epsilon();
  double h=std::max(hInit,hmin);
  double t=T0;
  VariableType ycurr=y0;
  double delta=T-T0;
  while (t<T && iter<=maxSteps)
    {
      ++iter;
      // The low precision solution
      VariableType ylow;
      // The high precision solution
      VariableType yhigh;
      // I compute the amount of error per time step
      // since I want to control the final error
      // But I also have to avoid overdoing, otherwise I will
      // never expand the step!
      double errorPerTimeStep=tol*h/delta;
      // Check if new time step will cross the final time step
      if(t+h>=T)
	{
	  h=T-t; // fix h
	  if (h<hmin) // test is new step very small
	    {
	      // step ridicuously small. We are at the end, stop here
	      ylow=ycurr;
	      yhigh=ycurr;
	    }
	  else
	    std::tie(ylow,yhigh)=RKFstep(t,ycurr,h); //last step
	}
      else
	    std::tie(ylow,yhigh)=RKFstep(t,ycurr,h); //step
      double currentError=(ylow-yhigh).norm();
      double mu=std::pow(errorPerTimeStep/currentError,factor);// very expensive:alternative take factor=1 always
      if(currentError<=errorPerTimeStep)
        {
          //fine set new point!
          t=t+h;
          time.push_back(t);
          y.push_back(yhigh);
          ycurr=yhigh;
          estimatedError+=currentError;
          // Expand next step if error very small, step not previously rejected and I am not at the end
          if((mu>=1.0) && !rejected && (t<T))
            {
              h*=std::min(expansionFactor,mu); //alternative use only expansion factor
              ++expansions;
            }
          rejected=false;
        }
      else
        {
          rejected=true;
          h*=mu*reductionFactor;// a little more to be sure. Alternative use only reductionFactor
          ++contractions;
          h= h<=hmin? hmin: h;
        }
    }
  if(iter>maxSteps) failed=true;
  return res;
  }

  // Note that I use trailing return type syntax here since otherwise I had to write
  //
  // std::pair<typename RKFTraits::VariableType, typename RKFTraits::VariableType>
  // RKFstep(const double & tstart, const VariableType & y0, const double& h) const
  //
  template<class B, class Model>
  auto RKFMC<B,Model>::RKFstep(const double & tstart, const VariableType & y0, const double& h) const
  -> std::pair<VariableType, VariableType>
  {
    auto constexpr Nstages=B::Nstages();
    std::array<VariableType,Nstages> K;
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
        VariableType value=y0;
        for (unsigned int j=0;j<i;++j)value+=A[i][j]*K[j];
        K[i]=M_f(time,value)*h;
      }
    VariableType v1=y0;
    VariableType v2=y0;
    for (unsigned int i=0; i<Nstages;++i)
      {
        v1+=K[i]*b1[i];
        v2+=K[i]*b2[i];
      }
    return std::make_pair(v1,v2);
  }

template <class VariableType>
std::ostream&
operator << (std::ostream& out, const RKFResult<VariableType> & res)
{
  out<<"# Number ot time steps:"<<res.time.size()<<" N. contractions:"<<res.contractions<<" N. expansions:"<<res.expansions<<std::endl;
  out<<"#   t    y   Estimated error="<<res.estimatedError<<std::endl;
  double hmin=res.time[1]-res.time[0];
  double hmax=hmin;
  for (unsigned int i=0;i<res.time.size()-1;++i)
    {
      auto delta=res.time[i+1]-res.time[i];
      hmax=std::max(hmax,delta);
      hmin=std::min(hmin,delta);
    }
  out<<"# hmin:"<<hmin<<" hmax:"<<hmax<<std::endl;
  std::size_t i=0;
  for (auto const & t: res.time)
    out<<t<<" "<<res.y[i++]<<"\n";
  return out;
}
}// end namespace multicity

}// end namespace apsc

#endif /* SRC_RK45_RKF_HPP_ */
