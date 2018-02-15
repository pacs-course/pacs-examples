#ifndef HH_MINRES___HH
#define HH_MINRES___HH
//*****************************************************************
// Iterative template routine -- MINRES
//
// CG solves the symmetric positive definite linear
// system Ax=b using the Preconditioned MINRES method.
// Preconditioner must be symmetric
//
// MINRES follows the algorithm described on p. 192 of the 
// Book by Elman et al: Finite elements and fast solution of
// linear systems, 2nd editions 
//
// The return value indicates convergence within max_iter (input)
// iterations (0), or no convergence within max_iter iterations (1).
//
// Upon successful return, output arguments have the following values:
//  
//        x  --  approximate solution to Ax = b
// max_iter  --  the number of iterations performed before the
//               tolerance was reached
//      tol  --  the residual after the final iteration
//  
//*****************************************************************
#include <cmath>
template < class Matrix, class Vector, class Preconditioner>
int 
MINRES(const Matrix &A, Vector &x, const Vector &b,
   const Preconditioner &M, int &max_iter, typename Vector::Scalar &tol)
{
  using Real= typename Matrix::Scalar;
  Vector v0(b.size());
  v0.setZero();
  Vector w0(b.size());
  w0.setZero();
  Vector w1(b.size());
  w1.setZero();
  Real gamma0(1.0);
  Real s0(0.0), s1(0.0);
  Real c0(1.0), c1(1.0);
  Real resid(0.0);

  // Set initial data
  Real normb  = b.norm();
  Vector v1   = b - A*x;
  if (normb == 0.0) normb = 1.0;
  // Test trivial solution
  resid= v1.norm()/normb;
  //std::cout<<resid<<std::endl;
  if (resid <= tol)
    {
      tol = resid;
      max_iter = 0;
      return 0;
    }
  Vector z1   = M.solve(v1);
  auto   dot  = z1.dot(v1);
  if(dot <=0.0 )
    throw std::runtime_error("MINRES: Precond. must be spd");
  Real gamma1 = std::sqrt(dot);
  auto eta    = gamma1;
  
  for (int i = 1; i <= max_iter; ++i)
    {
      z1         /= gamma1; // scale z(i)
      Vector Az   = A*z1;
      auto d1     = z1.dot(Az);
      // std::cout<<" It "<<i<<std::endl;
      // std::cout<<" gamma 1"<<gamma1<<std::endl;
      Vector v2   = Az - (d1/gamma1)*v1 - (gamma1/gamma0)*v0;
      Vector z2   = M.solve(v2);
      dot         = z2.dot(v2);
      if(dot <=0.0 )
        throw std::runtime_error("MINRES: Precond. must be spd");
      auto gamma2 = std::sqrt(dot);
      auto alpha0 = c1*d1 - c0*s1*gamma1;
      auto alpha1 = std::sqrt(alpha0*alpha0+gamma2*gamma2);
      auto alpha2 = s1*d1+c0*c1*gamma1;
      auto alpha3 = s0*gamma1;
      //std::cout<<" alpha 1"<<alpha1<<std::endl;
      //std::cout<<" alpha 2"<<alpha2<<std::endl;
      //std::cout<<" alpha 3"<<alpha3<<std::endl;
      auto inva1  = 1.0/alpha1;
      auto c2     = alpha0*inva1;
      auto s2     = gamma2*inva1;
      Vector w2   = inva1*(z1-alpha3*w0-alpha2*w1);
      auto f      = c2*eta;
      x          += f*w2; // update solution
      // Test convergence
      // \todo Here we should do better since the residual
      // should already be available
      resid = (b-A*x).norm()/normb;
      if (resid <= tol)
        {
          tol      = resid;
          max_iter = i;
          return 0;     
        }
      // Update values
      eta        *= -s2;
      gamma0      = gamma1;
      gamma1      = gamma2;
      s0          = s1;
      s1          = s2;
      w0          = w1;
      w1          = w2;
      z1          = z2;
      v0          = v1;
      v1          = v2;
      c0          = c1;
      c1          = c2;
    }
  // Exit with error flag set
  tol = (b-A*x).norm()/normb;;
  return 1;
}
#endif
