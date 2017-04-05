#include "numerical_integration.hpp"
namespace NumericalIntegration
{
  
  Quadrature::Quadrature( const QuadratureRule & rule,
			  const Geometry::Mesh1D & mesh ):
    M_rule(rule), 
    M_mesh(mesh)
  {}
  
  double Quadrature::apply(FunPoint f)
  {
    double result(0);
    int nnodes = M_rule.num_nodes();

    for(unsigned int i=0; i<M_mesh.numNodes() - 1; ++i)
    {
      double a = M_mesh[i];
      double b = M_mesh[i+1];
      double xm = (a + b)/2.;
      double wf = (b - a)/2.;
      for (int j=0; j<nnodes; ++j)
	{
	  double x = xm + wf * M_rule.node(j);
	  result += wf * f(x) * M_rule.weight(j);
      }
    }
    return result;
  }
}
