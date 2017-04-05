#include "numerical_rule.hpp"

namespace NumericalIntegration{

  QuadratureRule::QuadratureRule():_w(),_n(){};

  QuadratureRule::~ QuadratureRule(){}

  int QuadratureRule::num_nodes() const
  { return _w.size();}

  double QuadratureRule::node(const int i) const
  { return _n[i];}

  double QuadratureRule::weight(const int i) const
  { return _w[i];}
  
  Simpson::Simpson(){
    _w.push_back(1./3);
    _w.push_back(4./3);
    _w.push_back(1./3);
    _n.push_back(-1.0);
    _n.push_back(0.0);
    _n.push_back(1.0);
  }

  MidPoint::MidPoint(){
    _w.push_back(2.0);
    _n.push_back(0.0);
  }
}
