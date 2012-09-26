#include "numerical_integration.hpp"
namespace NumericalIntegration
{

  Quadrature::Quadrature( const QuadratureRule & rule,
                          const Geometry::Mesh1D & mesh ):
    M_rule(rule),
    M_mesh(mesh)
  {}

  double Quadrature::apply ( const FunPoint& f )
  {
    double result(0.);
    const unsigned int nnodes = M_rule.num_nodes();

    for( unsigned int i=0; i<M_mesh.numNodes() - 1; ++i )
    {
      const double a = M_mesh [ i ];
      const double b = M_mesh [ i+1 ];
      const double xm = (a + b)/2.;
      const double wf = (b - a)/2.;
      for ( unsigned int j=0; j<nnodes; ++j )
      {
        const double x = xm + wf * M_rule.node(j);
        result += wf * f(x) * M_rule.weight(j);
      }
    }
    return result;
  }
} // namespace NumericalIntegration
