/*
 * RKF.hpp
 *
 *  Created on: Feb 4, 2019
 *      Author: forma
 */

#ifndef SRC_RK45_RKF_HPP_
#define SRC_RK45_RKF_HPP_
#include "ButcherRKF.hpp" // not strictly needed here but it simplifies things
#include "JacobianFactory.hpp"
#include "Newton.hpp"
#include "RKFTraits.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <concepts>
#include <exception>
#include <functional>
#include <iostream>
#include <limits>
#include <tuple>
#include <utility>
#include <vector>
namespace apsc
{

/*!
 * A class for explicit or diagonally implicit Runge-Kutta-Fehlberg type
 * solution of ODEs
 * @tparam B The Butcher table of the scheme. Must be defined following the
 * scheme shown in ButcherRKF.hpp
 * @tparam KIND The type of traits to be used: SCALAR, VECTOR, MATRIX
 */
template <apsc::ButcherArrayConcept B, RKFKind KIND = RKFKind::SCALAR>
class RKF : public RKFTraits<KIND>
{
public:
  using VariableType = typename RKFTraits<KIND>::VariableType;
  using Function = typename RKFTraits<KIND>::ForcingTermType;
  /*!
   * Structure holding the results of the integration
   */
  struct RKFResult
  {
    //! Absolute time values (units as in integration interval)
    std::vector<double> time;
    //! Solution values (state variables) at each time step
    std::vector<VariableType> y;
    //! estimated error
    double estimatedError{0.0};
    //! Failure
    bool failed{false};
  //! Number of time-step expansions
    int expansions{0};
  //! Number of time-step contractions
    int contractions{0};
    /*
     * Provide a non-templated friend streaming operator for this concrete
     * RKFResult instantiation. Defining it here (inside the nested struct)
     * makes a dedicated non-template operator<< available for each
     * `RKF<B,KIND>::RKFResult` type which avoids template-deduction issues
     * that occur when trying to deduce the enclosing Butcher-table template
     * parameter from a nested dependent type.
     */
    friend std::ostream &
    operator<<(std::ostream &out, const RKFResult &res)
    {
      out << "# Number of time steps:" << res.time.size()
          << " N. contractions:" << res.contractions
          << " N. expansions:" << res.expansions << '\n';
      out << "#   t    y   Estimated error=" << res.estimatedError << '\n';
      double hmin = 0.0;
      double hmax = 0.0;
      if(res.time.size() >= 2)
        {
          hmin = res.time[1] - res.time[0];
          hmax = hmin;
          for(std::size_t i = 0; i + 1 < res.time.size(); ++i)
            {
              auto delta = res.time[i + 1] - res.time[i];
              hmax = std::max(hmax, delta);
              hmin = std::min(hmin, delta);
            }
        }
      out << "# hmin:" << hmin << " hmax:" << hmax << '\n';
      std::size_t i = 0;
      if constexpr(KIND == RKFKind::SCALAR)
        {
          for(auto const &t : res.time)
            out << t << " " << res.y[i++] << "\n";
        }
      else if constexpr(KIND == RKFKind::VECTOR || KIND == RKFKind::MATRIX)
        {
          for(auto const &t : res.time)
            {
              out << t << " ";
              auto const &yy = res.y[i++];
              for(int k = 0; k < yy.size(); ++k)
                out << yy(k) << " ";
              out << "\n";
            }
        }
      return out;
    }
  };

  //! Constructor just taking the function
  template <class F = Function>
    requires std::convertible_to<F, Function>
  RKF(F &&f) : M_f{std::forward<F>(f)}
  {}
  // Constructor passing butcher table and forcing function
  // Butcher table is now a constant expression
  // This constructor is there only to activate template parameter deduction
  template <class F = Function>
    requires std::convertible_to<F, Function>
  RKF(B const &bt, F &&f) : RKF{f}
  {}

  //! Default constructor
  RKF() = default;
  //! Set the forcing function
  void
  set_function(Function const &f)
  {
    M_f = f;
  }
  //! Set the Butcher Array
  // void
  // set_ButcherArray(B const &bt)
  //{
  //   ButcherTable = bt;
  // }

  /*!
   * @param T0 initial time
   * @param T  final time
   * @param y0 initial condition
   * @param hInit initial time step
   * @param tol desired global error max-norm
   * @param maxStep Safeguard to avoid too many steps (default 2000)
   * @todo It would be better to group the parameters tol and maxStep into an
   * (internal?) struct
   */
  [[nodiscard]] RKFResult operator()(double T0, double T,
                                     VariableType const &y0, double hInit,
                                     double tol = 1e-6,
                                     int    maxStep = 2000) const;
  /*!
   * @brief Default options for the quasi-Newton solver.
   * @details Kept public to simplify handling. Mutable because it can be
   * modified also on a const object. These options can be changed by the user
   * after constructing the RKF object. Designated initializers (C++20) are
   * used to make it clearer how each option is named in the struct.
   */
  apsc::NewtonOptions newtonOptions{
    // Designated initializers in action
    .tolerance = 1.e-10,       // tolerance for step in Newton
    .minRes = 1.e-10,          // tolerance on residual
    .maxIter = 100,            // max number of iterations
    .backtrackOn = true,       // use backtracking
    .stopOnStagnation = false, // do not stop on stagnation
    .alpha = 1.e-4, // parameter for 1st Wolfe condition (backtracking)
    .backstepReduction = 0.5, // Reduction coefficient (backtracking)
    .maxBackSteps = 4,        // Max number backstep
    .lambdaInit = 1.          // initial lambda
  };
  mutable apsc::Newton newtonSolver{apsc::make_Jacobian(apsc::JacobianKind::BroydenG),
                                    newtonOptions};

private:
  Function           M_f;
  static constexpr B ButcherTable = B{};
  /*! Function for a single step. It is private since is used only internally.
   *
   * @note
   * I have used trailing return type syntax, to be consistent with the
   * definition given outside this class template definition. But here it is not
   * necessary. I could have written
   * @code
   * std::pair<VariableType,VariableType> RKFstep(const double & tstart, const
   * VariableType & y0, const double & h)
   * @endcode
   * since in this context VariableType is well defined
   *
   * @param tstart start time
   * @param y0 value at tstart
   * @param h time step
   * @return values computed with the two rules
   */
  auto RKFstep(const double &tstart, const VariableType &y0,
               const double &h) const -> std::pair<VariableType, VariableType>;
};

//   ***********************************************
//   ******    IMPLEMENTATIONS OF TEMPLATE FUNCTIONS
//   ***********************************************

template <apsc::ButcherArrayConcept B, RKFKind KIND>
RKF<B, KIND>::RKFResult
RKF<B, KIND>::operator()(double T0, double T, const VariableType &y0,
                         double hInit, double tol, int maxSteps) const
{
  RKFResult res;
  // Useful alias to simplify typing
  std::vector<double>       &time = res.time;
  std::vector<VariableType> &y = res.y;
  auto                      &expansions = res.expansions;
  auto                      &contractions = res.contractions;
  auto                      &estimatedError = res.estimatedError;
  estimatedError = 0.0; // set initial error to zero
  auto &failed = res.failed;
  failed = false; // set failed to false
  // bool expanded =false; // keep track of expansions
  //  reserve some space according to data. It may help reduce memory
  //  reallocations
  int expectedSteps =
    std::min(std::max(1, 1 + static_cast<int>((T - T0) / hInit)), maxSteps);
  time.reserve(expectedSteps);
  y.reserve(expectedSteps);
  // push initial step
  time.push_back(T0);
  y.push_back(y0);
  // to check if a step has been rejected
  bool rejected(false);
  // safety factor if error greater than tolerance
  double constexpr reductionFactor = 0.98;
  // I expand at most expansionfactor h, for safety
  double constexpr expansionFactor = 4.;
  // I reduce at most a fraction, for safety
  double constexpr maxreduction = 0.1;
  //
  // Now I need a factor to specify when I can enlarge the time step
  // to avoid reducing and expanding the time step repeatedly
  // I need to take into account the order of the scheme may be >2
  double factor_contraction = 1. / (ButcherTable.order);
  double factor_expansion = 1. / (ButcherTable.order + 1.0);

  double timeInterval = T - T0;
  if(timeInterval <= 0)
    {
      failed = true;
      std::cerr << "Time interval must be greater than zero\n";
      return res;
    }

  // Iteration counter
  int iter = 0;
  // I want to check that the time step does not go ridiculously small
  // @todo make it a member variable!
  double hmin = 100 * timeInterval * std::numeric_limits<double>::epsilon();
  double h = std::max(hInit, hmin);
  double t = T0;
  VariableType ycurr = y0;
  bool         minimalh = false;
  // int oscilla=0;
  while(t < T && iter <= maxSteps)
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
      double errorPerTimeStep =
        ButcherTable.order == 1 ? tol : tol * (h / timeInterval);
      // double errorPerTimeStep=tol*h/timeInterval;
      // if (ButcherTable.implicit())
      //{
      //  auto options=newtonSolver.getOptions();
      //  options.minRes=errorPerTimeStep;
      //  options.tolerance=0.1*errorPerTimeStep;
      //  newtonSolver.setOptions(options);
      // Check if new time step will cross the final time step and we are not
      // expanding
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
        std::tie(yprimal, ytest) = RKFstep(t, ycurr, h); // step
        double currentError = this->norm(yprimal - ytest);
        double ratio = 0.0;
        if(currentError <= std::numeric_limits<double>::epsilon())
          ratio = std::numeric_limits<double>::infinity();
        else
          ratio = errorPerTimeStep / currentError;
        double mu = std::max(
          maxreduction,
          std::pow(ratio, factor_contraction)); // very expensive:alternative
                                                // take factor=1 always
        //     if(currentError<=errorPerTimeStep)
        if(ratio >= 1.0)
          {
            // fine set new point!
            t += h;
            time.push_back(t);
            y.push_back(yprimal);
            ycurr = yprimal;
            estimatedError += currentError;
            h = std::min(h, T - t);
            // Expand next step if error small, step not previously rejected and
            // I am not at the end
            // if(rejected)++oscilla;
            if(!rejected && (t < T))
              {
                auto mu2 = std::pow(ratio, factor_expansion);
                h *= std::min(expansionFactor,
                              mu2);     // alternative use only expansion factor
                h = std::min(h, T - t); // cannot go over T
                // expanded=true;
                ++expansions;
              }
            rejected = false;
          }
        else
          {
            // expanded=false;
            if(h <= hmin)
              {
                // we are at the minimum we have to accept it
                t = t + h;
                time.push_back(t);
                y.push_back(yprimal);
                ycurr = yprimal;
                estimatedError += currentError;
                rejected = false;
                minimalh = true;
              }
            else
              {
                // step is rejected!
                rejected = true;
                h *=
                  mu * reductionFactor; // a little more to be sure. Alternative
                                        // use only reductionFactor
                ++contractions;
                h = h <= hmin ? hmin : h;
              }
          }
      }
    }
  if(iter > maxSteps)
    {
      failed = true;
      std::cerr << "RKF: maximum number of steps exceeded\n";
    }
  if(minimalh)
    {
      std::cerr << "RKF used the minimal value for h=" << hmin
                << ". Error may be greater than expected.\n";
    }
  // std::cout<<"Oscillazioni="<<oscilla<<std::endl;
  return res;
}

// Note that I use trailing return type syntax here since otherwise I had to
// write
//
// std::pair<typename RKFTraits::VariableType, typename RKFTraits::VariableType>
// RKFstep(const double &tstart, const VariableType &y0, const double &h)
// const
//
template <apsc::ButcherArrayConcept B, RKFKind KIND>
auto
RKF<B, KIND>::RKFstep(const double &tstart, const VariableType &y0,
                      const double &h) const
  -> std::pair<VariableType, VariableType>
{
  auto constexpr Nstages = B::Nstages();
  std::array<VariableType, Nstages> K{};
  // They are constant expressions!
  auto const &A = ButcherTable.A;
  auto const &c = ButcherTable.c;
  auto const &b1 = ButcherTable.b1;
  auto const &b2 = ButcherTable.b2;
  //@todo Test if implicit, no KIND=MATRIX
  //@todo Identify if implicit outside this heavily used routine!
  for(std::size_t i = 0; i < Nstages; ++i)
    {
      double       time = tstart + c[i] * h;
      VariableType value = y0;
      for(std::size_t j = 0; j < i; ++j)
        value += A[i][j] * K[j];
      if constexpr(ButcherTable.implicit())
        {
          if constexpr(KIND == apsc::RKFKind::VECTOR)
            {
              // Implicit (at the moment I support only DIRK)
              auto fun = [&value, &time, &A, &i, &h,
                          this](VariableType const &x) -> VariableType {
                return M_f(time, value + h * A[i][i] * x) - x;
              };
              newtonSolver.setNonLinSys(fun);
              auto result = newtonSolver.solve(M_f(time, value));
              if(!result.converged)
                {
                  std::cerr << "Solution of the nonlinear problem failed\n";
                  std::cerr << "Last residual " << result.residualNorm
                            << std::endl;
                  std::cerr << "y0, value, h, time, stage, fun" << y0 << " "
                            << value << " " << h << " " << time << " " << i
                            << " " << M_f(time, value) << std::endl;
                  throw std::runtime_error("Newton iteration did not converge");
                }
              K[i] = result.solution * h;
            }
          else
            {
              throw std::runtime_error(
                "Cannot use implicit RK unless KIND is VECTOR");
            }
        }
      else
        K[i] = M_f(time, value) * h;
    }
  VariableType v1 = y0;
  VariableType v2 = y0;
  for(std::size_t i = 0; i < Nstages; ++i)
    {
      v1 += K[i] * b1[i];
      v2 += K[i] * b2[i];
    }
  return {v1, v2};
}

} // namespace apsc

#endif /* SRC_RK45_RKF_HPP_ */
