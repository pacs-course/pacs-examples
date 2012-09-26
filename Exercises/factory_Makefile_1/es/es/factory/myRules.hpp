#ifndef __MYRULES_HPP__
#define __MYRULES_HPP__
#include "numerical_rule.hpp"

namespace NumericalIntegration{
  class Simpson : public QuadratureRule
  {
  public:
    Simpson();
    virtual QuadratureRule * clone() const;
  };

  class MidPoint : public QuadratureRule
  {
  public:
    MidPoint();
    virtual QuadratureRule * clone()const ;
  };

  class Trapezoidal : public QuadratureRule
  {
  public:
    Trapezoidal();
    virtual QuadratureRule * clone()const ;
  };

  /*
    (-1,1) ->(a,b) x=y*(b-a)/2 + (a+b)/2
    wfactor dx/dy = h/2 
   */


}

#endif
