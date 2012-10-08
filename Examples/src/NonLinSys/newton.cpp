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
    unsigned int iter(0);
    double stepNorm;
    do{
      // Get jacobian
      auto J=jac(x);
      //
      // compute step=-J^-1(x) F(x)
      // A better code would check if the Jacobian is singular
      // and eventualy regularize it.
      //
      Eigen::VectorXd step=J.fullPivLu().solve(res);
      stepNorm=step.norm();
      x  += step;
      res = fSys.residual(x);
      resNorm = res.norm();
      ++iter;
    }  
    while(iter<opt.maxIter && resNorm>opt.minRes && stepNorm>opt.tolerance);
    // Check what happened
    bool converged(stepNorm<=opt.tolerance);
    return NewtonStatus{resNorm,iter,converged};
  }
}
