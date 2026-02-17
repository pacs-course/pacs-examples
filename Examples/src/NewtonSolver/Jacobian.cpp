/*
 * NewtonMethodsSupport.cpp
 *
 *  Created on: Feb 2, 2020
 *      Author: forma
 */
#include "Jacobian.hpp"
#include <cmath>
#include <limits>
#include <stdexcept>

apsc::DiscreteJacobian::ArgumentType
apsc::DiscreteJacobian::solve(const ArgumentType &x,
                              const ArgumentType &b) const
{
  using namespace apsc;
  if(!M_sys)
    throw std::runtime_error("ERROR: Non linear system not set in Jacobian");
  std::size_t                                M = x.size();
  apsc::DiscreteJacobian::JacobianMatrixType J(M, M);
  double                                     two_tol = 2 * tol;
  for(std::size_t i = 0; i < M; ++i)
    {
      ArgumentType x1(x);
      ArgumentType x2(x);
      x1[i] -= tol;
      x2[i] += tol;
      ArgumentType r1 = (*M_sys)(x1);
      ArgumentType r2 = (*M_sys)(x2);
      r2 -= r1;          // r2-r1 -> r2
      r2 = r2 / two_tol; // Finite difference
      J.col(i) = r2;     // fill i-th column
    }
  if(this->lambda != 0.0)
    {
      // add regularization
      Eigen::DiagonalMatrix<double, Eigen::Dynamic> I(M);
      I.setIdentity();
      J += lambda * I;
    }
  // I solve J d = b
  // @todo allow user to choose the solver. This solver is safe but slow
  return J.fullPivLu().solve(b);
}

apsc::FullJacobian::ArgumentType
apsc::FullJacobian::solve(const ArgumentType &x, ArgumentType const &b) const
{
  if(!Jac)
    throw std::runtime_error("ERROR: Jacobian function not set");
  apsc::FullJacobian::JacobianMatrixType J = this->Jac(x);
  return J.fullPivLu().solve(b);
}

apsc::BroydenB::ArgumentType
apsc::BroydenB::solve(const ArgumentType &x, const ArgumentType &r) const
{
  if(!M_sys)
    throw std::runtime_error("ERROR: Non linear system not set in Jacobian");
  // If I have not given the matrix, we take the identity
  if(firstTime)
    {
      if(B.rows() == 0)
        {
          ArgumentType dx = -r;
          ArgumentType dr = r - M_sys->operator()(x + dx);
          double       denom = dr.norm();
          if(denom <= std::numeric_limits<double>::epsilon())
            denom = 1.0;
          B = (dx.norm() / denom) *
              BroydenB::JacobianMatrixType::Identity(x.size(), x.size());
        }
      firstTime = false;
    }
  else
    {
      ArgumentType s = x - previousX;  // step
      ArgumentType dr = r - previousR; // residual difference
      auto denom = dr.squaredNorm();
      if(denom <= std::numeric_limits<double>::epsilon())
        denom = 1.0;
      auto factor = 1. / denom;
      B += factor * (s - B * dr) * dr.transpose();
    }
  previousX = x;
  previousR = r;
  return B * r;
}

apsc::BroydenG::ArgumentType
apsc::BroydenG::solve(const ArgumentType &x, const ArgumentType &r) const
{
  if(!M_sys)
    throw std::runtime_error("ERROR: Non linear system not set in Jacobian");
  // If I have not given the matrix, we take the identity
  if(firstTime)
    {
      if(B.rows() == 0)
        {
          ArgumentType dx = -r;
          ArgumentType dr = r - M_sys->operator()(x + dx);
          double       denom = dr.norm();
          if(denom <= std::numeric_limits<double>::epsilon())
            denom = 1.0;
          B = (dx.norm() / denom) *
              BroydenG::JacobianMatrixType::Identity(x.size(), x.size());
        }
      firstTime = false;
    }
  else
    {
      ArgumentType s = x - previousX;  // step delta
      ArgumentType dr = r - previousR; // residual difference
      auto denom = (s.transpose() * B * dr).value();
      if(std::abs(denom) <= std::numeric_limits<double>::epsilon())
        denom = 1.0;
      auto factor = 1. / denom;
      B += factor * (s - B * dr) * (s.transpose() * B);
    }
  previousX = x;
  previousR = r;
  return B * r;
};

apsc::Eirola_Nevanlinna::ArgumentType
apsc::Eirola_Nevanlinna::solve(const ArgumentType &x,
                               const ArgumentType &r) const
{
  if(!M_sys)
    throw std::runtime_error("ERROR: Non linear system not set in Jacobian");
  // If I have not given the matrix, we take the identity
  if(firstTime)
    {
      if(B.rows() == 0)
        {
          ArgumentType dx = -r;
          ArgumentType dr = r - M_sys->operator()(x + dx);
          double       denom = dr.norm();
          if(denom <= std::numeric_limits<double>::epsilon())
            denom = 1.0;
          B =
            (dx.norm() / denom) *
            Eirola_Nevanlinna::JacobianMatrixType::Identity(x.size(), x.size());
        }
      firstTime = false;
    }
  else
    {
      ArgumentType p = -B * r;
      ArgumentType q = M_sys->operator()(x + p) - r;
      auto denom = (p.transpose() * B * q).value();
      if(std::abs(denom) <= std::numeric_limits<double>::epsilon())
        denom = 1.0;
      auto factor = 1. / denom;
      B += factor * (p - B * q) * (p.transpose() * B);
    }
  previousX = x;
  previousR = r;
  return B * r;
};
