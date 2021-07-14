/*
 * NewtonMethodsSupport.cpp
 *
 *  Created on: Feb 2, 2020
 *      Author: forma
 */
#include "Jacobian.hpp"

apsc::DiscreteJacobian::ArgumentType
 apsc::DiscreteJacobian::solve (const ArgumentType& x, const ArgumentType & b) const
 {
  using namespace apsc;
  std::size_t M = x.size();
  apsc::DiscreteJacobian::JacobianMatrixType J(M,M);
  for (std::size_t i=0; i<M;++i)
    {
      ArgumentType x1(x);
      ArgumentType x2(x);
      x1[i] -= M_tol;
      x2[i] += M_tol;
      ArgumentType r1 = (*M_sys)(x1);
      ArgumentType r2 = (*M_sys)(x2);
      r2 -= r1; // r2-r1 -> r2
      r2  = r2 / (2*M_tol); // Finite difference
      J.col(i) = r2; // fill i-th column
    }
  if(this->lambda !=0.0)
    {
      // add regularization
      Eigen::DiagonalMatrix<double,Eigen::Dynamic> I;
      I.setIdentity(M);
      J += lambda*I;
    }
    return J.fullPivLu().solve(b);
}

apsc::FullJacobian::ArgumentType
apsc::FullJacobian::solve (const ArgumentType & x, ArgumentType const & b) const
{
  apsc::FullJacobian::JacobianMatrixType J=this->Jac(x);
  return J.fullPivLu().solve(b);
}

apsc::BroydenB::ArgumentType
apsc::BroydenB::solve (const ArgumentType& x, const ArgumentType& r) const
{
  // If I have not given the matrix, we take the identity
  if(firstTime)
    {
      if(B.rows()==0)
        {
          ArgumentType dx=-r;
          ArgumentType dr=r-M_sys->operator()(x+dx);
          B=(dx.norm()/dr.norm())* BroydenB::JacobianMatrixType::Identity(x.size(),x.size());
        }
      firstTime=false;
    }
  else
    {
      ArgumentType s=x-previousX; // step
      ArgumentType dr=r-previousR; // residual difference
      auto factor=1./(dr.norm()*dr.norm());
      B += factor*(s-B*dr)*dr.transpose();
    }
  previousX=x;
  previousR=r;
  return B *r;
};

apsc::BroydenG::ArgumentType
apsc::BroydenG::solve (const ArgumentType& x, const ArgumentType& r) const
{
  // If I have not given the matrix, we take the identity
  if(firstTime)
    {
      if(B.rows()==0)
        {
          ArgumentType dx=-r;
          ArgumentType dr=r-M_sys->operator()(x+dx);
          B=(dx.norm()/dr.norm())*BroydenG::JacobianMatrixType::Identity(x.size(),x.size());
        }
      firstTime=false;
    }
  else
    {
      ArgumentType s=x-previousX; // step delta
      ArgumentType dr=r-previousR; // residual difference
      auto factor=1./(s.transpose()*B*dr);
      B += factor*(s-B*dr)*(s.transpose()*B);
    }
  previousX=x;
  previousR=r;
  return B *r;
};

apsc::Eirola_Nevanlinna::ArgumentType
apsc::Eirola_Nevanlinna::solve (const ArgumentType& x, const ArgumentType& r) const
{
  // If I have not given the matrix, we take the identity
  if(firstTime)
    {
      if(B.rows()==0)
        {
          ArgumentType dx=-r;
          ArgumentType dr=r-M_sys->operator()(x+dx);
          B=(dx.norm()/dr.norm())*Eirola_Nevanlinna::JacobianMatrixType::Identity(x.size(),x.size());
        }
      firstTime=false;
    }
  else
    {
      ArgumentType p =-B*r;
      ArgumentType q= M_sys->operator()(x+p)-r;
      auto factor=1./(p.transpose()*B*q);
      B += factor*(p-B*q)*(p.transpose()*B);
    }
  previousX=x;
  previousR=r;
  return B *r;
};


