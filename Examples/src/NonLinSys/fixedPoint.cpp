#include "fixedPoint.hpp"
namespace NonLinearSystems{
  FixedPointStatus fixedPoint(NonLinSys const & fSys,
		      argumentType & x,
		      const FixedPointOptions opt)
  {
    if(fSys.numEq()!=x.size()) return  FixedPointStatus{0,0,false};
    // Check if we are already at zero
    auto res=fSys.residual(x);
    double resNorm=res.norm();
    if(resNorm<=opt.minRes) return FixedPointStatus{resNorm,0,true};
    // Start Newton iterations
    unsigned int iter(0);
    do{
      // compute step=- alpha*F(x)
      // A better code would check if the Jacobian is singular
      // and eventualy regularize it.
      //
      auto step=opt.alpha*res;
      x  += step;
      res = fSys.residual(x);
      resNorm = res.norm();
      ++iter;
    }  
    while(iter<opt.maxIter && resNorm>opt.minRes);
    // Check what happened
    bool converged(resNorm<=opt.minRes);
    return FixedPointStatus{resNorm,iter,converged};
  }
}
