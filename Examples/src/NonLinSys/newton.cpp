#include "newton.hpp"
namespace NonLinearSystems{
  NewtonStatus Newton(NonLinSys const & fSys,
		      Jacobian const & jac,
		      argumentType & x,
		      const NewtonOptions opt)
  {
    if(fSys.numEq()!=x.size()) return  NewtonStatus{0,0,false};
    // Check if we are already at zero
    auto res=fSys.residual(x);
    double resNorm=res.norm();
    if(resNorm<=opt.minRes) return NewtonStatus{resNorm,0,true};
    // Start Newton iterations
    unsigned int iter{0};
    double stepNorm{2.0*opt.tolerance};
    do{
      // Get jacobian
      auto J=jac(x);
      //
      // compute step=-J^-1(x) F(x)
      // A better code would check if the Jacobian is nearly singular
      // and eventualy regularize it.
      //
      Eigen::VectorXd step=J.fullPivLu().solve(res);
      stepNorm=step.norm();
      // Start Backtracking 
      double lambda=1.0;
      unsigned int backiter{0u};
      auto resNormB=resNorm;
      while (
             // a nice use of the comma operator to save operations 
             (res=fSys.residual(x+lambda*step),resNorm=res.norm()) >=
             (1.0-lambda*opt.alpha)*resNormB
             &&
             (backiter++ < opt.maxback))
        {
          lambda *= opt.backstep;
        }// end backtracking
      x  += lambda*step;
    }
    // End main loop
    while(iter++<opt.maxIter && resNorm>opt.minRes && stepNorm>opt.tolerance);
    // Check what happened
    bool converged(stepNorm<=opt.tolerance);
    return NewtonStatus{resNorm,iter,converged};
  }
}
