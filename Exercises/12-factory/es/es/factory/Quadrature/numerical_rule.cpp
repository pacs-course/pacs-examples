#include "numerical_rule.hpp"

namespace NumericalIntegration{
  QuadratureRule::QuadratureRule():_w(),_n(), my_order(0){};
  int  QuadratureRule::num_nodes() const {return _n.size();}
  double  QuadratureRule::node(const int i) const {return _n[i];}
  double  QuadratureRule::weight(const int i)const {return _w[i];}
  unsigned int  QuadratureRule::order()const {return my_order;}
}
