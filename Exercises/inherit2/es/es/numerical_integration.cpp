#include "numerical_integration.hpp"
namespace NumericalIntegration
{

  Quadrature::Quadrature ( const QuadratureRule & rule,
                           double const & a,
                           double const & b,
                           unsigned int const & nint ):
    M_rule(rule),
    M_a(a),
    M_b(b),
    M_h((b-a)/static_cast<double>(nint)),
    M_wf(M_h/2.0),
    M_nint(nint),
    M_knots(nint+1)
  {
    M_knots[0] = M_a;
    for(unsigned int i=1; i<M_knots.size(); ++i)
    {
      M_knots[i] = M_knots[i-1] + M_h;
    }
  }

  double Quadrature::apply ( const FunPoint& f ) const
  {
    double result(0);
    const unsigned int nnodes = M_rule.num_nodes();

    for ( unsigned int i=0; i<M_knots.size() - 1; ++i )
    {
      const double a = M_knots[i];
      const double b = M_knots[i+1];
      const double xm = (a + b)/2.0;
      for ( unsigned int j=0; j<nnodes; ++j)
      {
         const double x = xm + M_wf * M_rule.node(j);
         result += M_wf*f(x)*M_rule.weight(j);
      }
    }
    return result;
  }
} // namespace NumericalIntegration
