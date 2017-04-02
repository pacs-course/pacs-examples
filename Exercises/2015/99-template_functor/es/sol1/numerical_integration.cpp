#include "numerical_integration.hpp"

namespace NumericalIntegration
{
 
  Quadrature::Quadrature(const Mesh1D & mesh):_mesh(mesh){}
    
  double Quadrature::apply(FunPoint const & f)
  {
    
    double result(0);
    int nnodes=_rule.num_nodes();
    for(unsigned int i=0;i<_mesh.numNodes()-1;++i){
      double a=_mesh[i];
      double b=_mesh[i+1];
      double xm=(a+b)*0.5;
      double wf=(b-a)*0.5;
      for (int j=0;j<nnodes;++j){
	double x=xm+wf*_rule.node(j);
	result+= wf*f(x)*_rule.weight(j);
      }
    }
    return result;
  }
 
}
