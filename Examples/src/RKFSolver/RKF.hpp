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
#include <iostream>
#include "ButcherRKF.hpp" // not strictly needed here but it simplifies things
#include "Newton.hpp"
#include "JacobianFactory.hpp"
#include "RKFTraits.hpp"
namespace apsc
{
  /*!
   * Structure holding the results of the integration
   *
   * @todo It can be made an internal structure of RKF. Indeed, it's use is intrinsically linked with the RKF class.
   * @tparam KIND The type of traits to be used
   */
  template <RKFKind KIND>
  struct RKFResult
  {
    //! Time steps
    std::vector<double> time;
    //!values
    std::vector<typename RKFTraits<KIND>::VariableType> y;
    //! estimated error
    double estimatedError{0.0};
    //! Failure
    bool failed{false};
    //! Number of time step expansions
    int expansions{0};
    //! Number of time step contractions
    int contractions{0};
  };

  /*!
   * A calss for explicit Runge-Kutta Fehlberg type solution of ODEs
   * @tparam B The Butcher table of the scheme. Must be defined following the scheme shown in ButcherRKF.hpp
   * @tparam KIND The type of traits to be used: SCALAR, VECTOR, MATRIX
   */
  template<class B,RKFKind KIND=RKFKind::SCALAR>
  class RKF : public RKFTraits<KIND>
  {
  public:
    using VariableType=typename RKFTraits<KIND>::VariableType;
    using Function= typename RKFTraits<KIND>::ForcingTermType;
    //! Constructor just taking the function
    RKF(Function const & f):M_f{f}{};
    //! Constructor passing butcher table and forcing function
    RKF(B const & bt, Function const & f):M_f{f},ButcherTable(bt){};
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
     * @todo It would be better to group the parameters tol and maxStep into an (internal?) struct
     */
    RKFResult<KIND>
    operator()(double const & T0, double const & T, VariableType const & y0,
               double const & hInit, double const & tol, int maxStep=2000) const;
    /*!
     * Kept public to simplify handling
     * Mutable because I should be free to modify it also on a const object
     */
    mutable apsc::Newton newtonSolver{apsc::make_Jacobian(apsc::BROYDENG),newtonOptions};
  private:
    Function M_f;
    B ButcherTable;
    // The default options for the quasi newton solver
    static constexpr
    apsc::NewtonOptions newtonOptions{
      1.e-10, // tolerance on step \f$||x_{new}-x_{old}||<tolerance\$
      1.e-10, // tolerance on residual
      100, // max iterations
      true, // backtracking is on
      false, // don't stop on stagnation
      1.e-4,// parameter for 1st wolfe condition (backtracking)
      0.5,// Reduction coefficient (backtracking)
      4,// Max number backstep
      1. // initial lambda
    };
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
  inline std::ostream & operator << (std::ostream & out, RKFResult<RKFKind::SCALAR> const & res);
  inline std::ostream & operator << (std::ostream & out, RKFResult<RKFKind::VECTOR> const & res);

  //   ***********************************************
  //   ******    IMPLEMENTATIONS OF TEMPLATE FUNCTIONS
  //   ***********************************************

  template<class B, RKFKind KIND>
  RKFResult<KIND>
  RKF<B,KIND>::operator () (const double& T0, const double& T,
                            const VariableType & y0, const double& hInit, const double& tol,
                            int maxSteps) const
  {
    RKFResult<KIND> res;
    // Useful alias to simplify typing
    std::vector<double> & time=res.time;
    std::vector<VariableType> & y=res.y;
    auto & expansions=res.expansions;
    auto & contractions=res.contractions;
    auto & estimatedError=res.estimatedError;
    estimatedError=0.0; // set initial error to zero
    auto & failed=res.failed;
    failed=false; // set failed to false
    //bool expanded =false; // keep track of expansions
    //  reserve some space according to data. It may help reduce memory reallocations
    int expectedSteps=std::min(std::max(1,1+static_cast<int>((T-T0)/hInit)),maxSteps);
    time.reserve(expectedSteps);
    y.reserve(expectedSteps);
    // push initial step
    time.push_back(T0);
    y.push_back(y0);
    // to check if a step has been rejected
    bool rejected(false);
    // safety factor if error greater than tolerance
    double constexpr reductionFactor=0.98;
    // I expand at most expansionfactor h, for safety
    double constexpr expansionFactor=4.;
    // I reduce at most a fraction, for safety
    double constexpr maxreduction=0.1;
    //
    // Now I need a factor to specify when I can enlarge the time step
    // to avoid reducing and expanding the time step repeatedly
    // I need to take into account the order of the scheme is >2
    double factor_contraction=1./(ButcherTable.order);
    double factor_expansion=1./(ButcherTable.order+1.0);

    double timeInterval=T-T0;
    if(timeInterval <=0)
      {
        std::cerr<<"Time interval must me greater than zero\n";
        return res;
      }

    // Iteration counter
    int iter=0;
    // I want to check that the time step does not go ridiculosly small
    // @todo make it a variable member!
    double hmin=100*timeInterval*std::numeric_limits<double>::epsilon();
    double h=std::max(hInit,hmin);
    double t=T0;
    VariableType ycurr=y0;
    bool minimalh=false;
    //int oscilla=0;
    while (t<T && iter<=maxSteps)
      {
        ++iter;
        // The low precision solution
        VariableType yprimal;
        // The high precision solution
        VariableType ytest;
        // I compute the amount of error per time step
        // since I want to control the final error
        // But I also have to avoid overdoing, so for low order
        // schemes I use tol to control the LTE and not tol*h
        double errorPerTimeStep= ButcherTable.order==1 ?tol:tol*(h/timeInterval);
        //double errorPerTimeStep=tol*h/timeInterval;
        //if (ButcherTable.implicit())
        //{
        //  auto options=newtonSolver.getOptions();
        //  options.minRes=errorPerTimeStep;
        //  options.tolerance=0.1*errorPerTimeStep;
        //  newtonSolver.setOptions(options);
        // Check if new time step will cross the final time step and we are not expandin
        /* if(t+h>=T && !expanded)
          {
            h=T-t; // fix h
            if (h<hmin) // test is new step very small
              {
                // step ridicuously small. We are at the end, stop here
                yprimal=ycurr;
                ytest=ycurr;
              }
            else
              {
                std::tie(yprimal,ytest)=RKFstep(t,ycurr,h); //last step
              }
            t=T;
            time.push_back(t);
            y.push_back(yprimal);
            estimatedError+=this->norm(yprimal-ytest);
          }
        else // perform next step
        */
          {
            std::tie(yprimal,ytest)=RKFstep(t,ycurr,h); //step
            double currentError=this->norm(yprimal-ytest);
            double ratio = errorPerTimeStep/currentError;
            double mu=std::max(maxreduction,std::pow(ratio,factor_contraction));// very expensive:alternative take factor=1 always
            //     if(currentError<=errorPerTimeStep)
            if(ratio>=1.0)
              {
                //fine set new point!
                t+=h;
                time.push_back(t);
                y.push_back(yprimal);
                ycurr=yprimal;
                estimatedError+=currentError;
                h = std::min(h,T-t);
                // Expand next step if error small, step not previously rejected and I am not at the end
                //if(rejected)++oscilla;
                if(!rejected && (t<T))
                  {
                    auto mu2=std::pow(ratio,factor_expansion);
                    h*=std::min(expansionFactor,mu2); //alternative use only expansion factor
                    h=std::min(h,T-t);// cannot go over T
                    //expanded=true;
                    ++expansions;
                  }
                rejected=false;
              }
            else
              {
                //expanded=false;
                if (h<=hmin)
                  {
                    // we are at the minimum we have to accept it
                    t=t+h;
                    time.push_back(t);
                    y.push_back(yprimal);
                    ycurr=yprimal;
                    estimatedError+=currentError;
                    rejected=false;
                    minimalh=true;
                  }
                else
                  {
                    // step is rejected!
                    rejected=true;
                    h*=mu*reductionFactor;// a little more to be sure. Alternative use only reductionFactor
                    ++contractions;
                    h= h<=hmin? hmin: h;
                  }
              }
          }
      }
    if(iter>maxSteps){
        failed=true;
        std::cerr<<"RKF: Max number of steps exceeded\n";
    }
    if(minimalh){
        std::cerr<<"RKF used minimal value for h="<<hmin<<" Error may be grater than expected\n";
    }
    //std::cout<<"Oscillazioni="<<oscilla<<std::endl;
    return res;
  }

  // Note that I use trailing return type syntax here since otherwise I had to write
  //
  // std::pair<typename RKFTraits::VariableType, typename RKFTraits::VariableType>
  // RKFstep(const double & tstart, const VariableType & y0, const double& h) const
  //
  template<class B, RKFKind KIND>
  auto RKF<B,KIND>::RKFstep(const double & tstart, const VariableType & y0, const double& h) const
  -> std::pair<VariableType, VariableType>
  {
    auto constexpr Nstages=B::Nstages();
    std::array<VariableType,Nstages> K;
    // I use references to simplify typing
    typename B::Atable const & A=ButcherTable.A;
    std::array<double,Nstages> const & c{ButcherTable.c};
    std::array<double,Nstages> const & b1{ButcherTable.b1};
    std::array<double,Nstages> const & b2{ButcherTable.b2};
    //@todo Test if implicit no KIND=MATRIX
    //@todo Identify if implicit outside this heavily used routine!
    for (unsigned int i=0;i<Nstages;++i)
      {
        double time=tstart+c[i]*h;
        VariableType value=y0;
        for (unsigned int j=0;j<i;++j)value+=A[i][j]*K[j];
        if (A[i][i]!=0.0)
          {
            if constexpr (KIND==apsc::RKFKind::VECTOR)
                    {
                // implicit (at the moment I support only DIRK!
                auto fun= [&value,&time,&A,&i,&h,this] (VariableType const& x)
                        ->VariableType
                        {return M_f(time,value+h*A[i][i]*x)-x; };
                newtonSolver.setNonLinSys(fun);
                auto result = newtonSolver.solve(M_f(time,value));
                if (!result.converged)
                  {
                    std::cerr<<"Solution of non-linear problem failed\n";
                    std::cerr<<"Last residual "<<result.residualNorm<<std::endl;
                  }
                K[i]=result.solution*h;
                    }
            else
              {
                std::cerr<<" cannot use implicit RK if KIND is not VECTOR\n";
                std::exit(1);
              }
          }
        else
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

  std::ostream&
  operator << (std::ostream& out, const RKFResult<RKFKind::SCALAR> & res)
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

  std::ostream&
  operator << (std::ostream& out, const RKFResult<RKFKind::VECTOR> & res)
  {
    out<<"# Number ot time steps:"<<res.time.size()<<" N. contractions:"<<res.contractions<<" N. expansions:"<<res.expansions<<std::endl;
    out<<"#   t    y(0)...   Estimated error="<<res.estimatedError<<std::endl;
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
      {
        out<<t<<" ";
        apsc::RKFTraits<RKFKind::VECTOR>::VariableType const & yy = res.y[i];
        ++i;
        for (int k =0; k< yy.size(); ++k)
          out<<yy[k]<<" ";
        out<<"\n";
      }
    return out;
  }


}// end namespace

#endif /* SRC_RK45_RKF_HPP_ */
