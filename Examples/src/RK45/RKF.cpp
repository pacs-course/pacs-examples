/*
 * RKF.cpp
 *
 *  Created on: Feb 4, 2019
 *      Author: forma
 */
#include "RKF.hpp"
#include<iostream>

std::ostream&
operator << (std::ostream& out, const ODE::RKFResult& res)
{
  out<<"#   t    y   Estimated error="<<res.estimatedError<<std::endl;
  std::size_t i=0;
  for (auto const & t: res.time)
    out<<t<<" "<<res.y[i++]<<std::endl;
  return out;
}
