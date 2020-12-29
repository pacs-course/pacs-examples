/*
 * DescentDirections.cpp
 *
 *  Created on: Dec 28, 2020
 *      Author: forma
 */
#include "DescentDirections.hpp"
#include "Eigen/Dense"

apsc::LineSearch_traits::Vector
apsc::BFGSDirection::operator()(const apsc::OptimizationCurrentValues &values)
{
  // First time is a gradient step
  if (firstTime)
    {
      auto n = values.currentPoint.size();
      H = Eigen::MatrixXd::Identity(n,n);
      firstTime=false;
      this->previousValues=values;
      return -values.currentGradient;
    }

  apsc::LineSearch_traits::Vector g =  values.currentGradient;
  apsc::LineSearch_traits::Vector yk = g - this->previousValues.currentGradient;
  apsc::LineSearch_traits::Vector sk = values.currentPoint-this->previousValues.currentPoint;
  auto yks=yk.dot(sk);
  // Correct approximate Hessian only if we maintain sdp property if not keep the old one
  if(yks > this->smallNumber*sk.norm()*yk.norm())
    {
      apsc::LineSearch_traits::Vector Hs;
      Hs=H*sk;
      H+=(yk*yk.transpose())/yks - (Hs*Hs.transpose())/(sk.dot(Hs));
    }
  this->previousValues=values;
  apsc::LineSearch_traits::Vector d = H.fullPivLu().solve(-g);
  return d;
}

void
apsc::BFGSDirection::reset()
{
this->firstTime=true;
}

/*                   BFGS with approximate inverse */
apsc::LineSearch_traits::Vector
apsc::BFGSIDirection::operator()(const apsc::OptimizationCurrentValues &values)
{
  // First time is a gradient step
  if (firstTime)
    {
      auto n = values.currentPoint.size();
      H = Eigen::MatrixXd::Identity(n,n);
      firstTime=false;
      this->previousValues=values;
      return -values.currentGradient;
    }

  apsc::LineSearch_traits::Vector g =  values.currentGradient;
  apsc::LineSearch_traits::Vector yk = g - this->previousValues.currentGradient;
  apsc::LineSearch_traits::Vector sk = values.currentPoint-this->previousValues.currentPoint;
  auto yks=yk.dot(sk);
  // Correct approximate Hessian only if we maintain sdp property if not keep the old one
  if(yks > this->smallNumber*sk.norm()*yk.norm())
    {
      H+=sk*sk.transpose()*(yks + yk.transpose()*H*yk)/(yks*yks)- (H*yk*sk.transpose()+sk*yk.transpose()*H)/yks;
    }
  this->previousValues=values;
  apsc::LineSearch_traits::Vector d = -H*g;
  return d;
}

void
apsc::BFGSIDirection::reset()
{
this->firstTime=true;
}

/*
 * Barzilai-Borwain
 */
apsc::LineSearch_traits::Vector
apsc::BBDirection::operator()(const apsc::OptimizationCurrentValues &values)
{
  // First time is a gradient step
  if (firstTime)
    {
      firstTime=false;
      this->previousValues=values;
      return -values.currentGradient;
    }

  apsc::LineSearch_traits::Vector g =  values.currentGradient;
  apsc::LineSearch_traits::Vector yk = g - this->previousValues.currentGradient;
  apsc::LineSearch_traits::Vector sk = values.currentPoint-this->previousValues.currentPoint;
  auto yks=yk.dot(sk);
  auto ykk=yk.dot(yk);
  this->previousValues=values;
  // Correct approximate Hessian only if we maintain sdp property if not keep the old one
  if(yks > this->smallNumber*sk.norm()*yk.norm() and ykk > smallNumber)
    {
      // I use a mix of the two possible strategies
      return -0.5*(yks/ykk+sk.dot(sk)/yks)*g;
      // Strategy 1
      //return -(yks/ykk)*g;
      // Strategy 2
      //return -(sk.dot(sk)/yks)*g;
  }
  else
    {
      return -g;
    }
}
