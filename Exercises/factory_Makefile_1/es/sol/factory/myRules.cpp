#include "rulesProxy.hpp"
#include "myRules.hpp"
namespace NumericalIntegration{
  
  Simpson::Simpson(){
    _w.push_back(1./3);
    _w.push_back(4./3);
    _w.push_back(1./3);
    _n.push_back(-1.0);
    _n.push_back(0.0);
    _n.push_back(1.0);
    my_order=5;
  }

  QuadratureRule * Simpson::clone()const { return new Simpson(*this);}
  
  MidPoint::MidPoint(){
    _w.push_back(2.0);
    _n.push_back(0.0);
    my_order=3;
  }

  QuadratureRule * MidPoint::clone() const { return new MidPoint(*this);}

  Trapezoidal::Trapezoidal(){
    _w.push_back(1.0);
    _w.push_back(1.0);
    _n.push_back(-1.0);
    _n.push_back(1.0);
    my_order=3;
  }
  
  QuadratureRule * Trapezoidal::clone() const { return new Trapezoidal(*this);}
}

// Registrazione
namespace
{
  using namespace NumericalIntegration;
  
  RuleProxy<Simpson> SH("Simpson");
  
  RuleProxy<Trapezoidal> TH("Trapezoidal");
  
  RuleProxy<MidPoint> MH("MidPoint");
}
