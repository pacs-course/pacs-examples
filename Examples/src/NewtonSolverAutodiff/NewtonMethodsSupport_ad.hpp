/*! \file NewtonMethodsSupport.hpp */
#ifndef NEWTON_SOLVER_AUTODIFF_SUPPORT_HPP
#define NEWTON_SOLVER_AUTODIFF_SUPPORT_HPP

#include "NewtonTraits_ad.hpp"

namespace apsc
{
namespace newton_ad
{
  //! Options controlling the Newton iteration.
  struct NewtonOptions
  {
    double tolerance{
      1.e-8}; //!< Tolerance for the step length. If the step length is smaller
              //!< than this value, the iteration stops.
    double minRes{1.e-6}; //!< Tolerance for the residual. If the residual is
                          //!< smaller than this value, the iteration stops.
    unsigned int maxIter{50}; //!< Maximum number of iterations. If the
                              //!< iteration reaches this number, it stops.
    bool backtrackOn{
      false}; //!< If true, backtracking is used to find a suitable step length.
              //!< If false, the full Newton step is used.
    bool stopOnStagnation{
      false}; //!< If true, the iteration stops if the residual does not
              //!< decrease for two consecutive iterations.
    double alpha{
      1.e-4}; //!< Parameter for the backtracking line search. The new residual
              //!< must be smaller than (1 - alpha * lambda) *
              //!< previousResidualNorm, where lambda is the step length.
    double backstepReduction{
      0.5}; //!< Factor by which the step length is reduced during backtracking.
            //!< The new step length is lambda * backstepReduction.
    unsigned int maxBackSteps{
      4}; // <! Maximum number of backtracking steps. If the iteration reaches
          // this number, it stops.
    double lambdaInit{
      1.0}; //<! Initial step length for the backtracking line search.
  };

  //! Result returned by Newton::solve.
  struct NewtonResult
  {
    NewtonTraits::ReturnType
           solution;          //!< The solution found by the Newton iteration.
    double residualNorm{0.0}; //!< The norm of the residual at the solution.
    double stepLength{
      0.0}; //!< The length of the last step taken by the Newton iteration.
    unsigned int iterations{
      0u}; //!< The number of iterations performed by the Newton iteration.
    bool converged{
      false}; //!< True if the Newton iteration converged, false otherwise.
    bool stagnation{false}; //!< True if the Newton iteration stopped due to
                            //!< stagnation, false otherwise.
  };

} // namespace newton_ad
} // namespace apsc

#endif
