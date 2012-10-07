#include "jacob.hpp"
namespace NonLinearSystems{
  //! Initialization of the default tolerance.
  double const 
  DiscreteJacobian::defaultTol(1.e3*std::numeric_limits<double>::epsilon());

  jacobianType
  DiscreteJacobian::operator() (argumentType const & x) const{
    // Find system dimensions
    // Beware x must be dimensioned correctly!
    auto numArgs=x.size();
    auto numEqs=this->M_sys->numEq();
    // Create an empty matrix with right dimensions
    jacobianType tmp(numEqs,numArgs);
    for ( unsigned int i=0; i<numArgs; ++i){
      argumentType x1(x);
      argumentType x2(x);
      x1[i] -= M_tol;
      x2[i] += M_tol;
      auto r1 = (*M_sys)(x1);
      auto r2 = (*M_sys)(x2);
      // I use Eigen overloaded operators
      r2 -= r1; // r2-r1 -> r2
      r2  = r2 / (2*M_tol); // Finite difference
      tmp.col(i) = r2; // fill i-th column
    }
    return tmp;
  }
}
