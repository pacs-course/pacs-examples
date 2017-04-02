#ifndef __NUMERICAL_INTEGRATION_HPP
#define __NUMERICAL_INTEGRATION_HPP
#include <vector>
#include "numerical_rule.hpp"
#include "mesh.hpp"
namespace NumericalIntegration{
  using Geometry::Mesh1D;

  typedef double(*FunPoint)(double const &);

  class Quadrature
  {
  public:
    // mesh 1D has to be copy contructible
    Quadrature(const Mesh1D &);

    // Compute the integral on function f
    template<typename FUN>
    double apply(FUN const & f);

  protected:

    const QuadratureRule _rule;
    Mesh1D _mesh;
  };

  /*
    (-1,1) ->(a,b) y*(b-a)/2 + (a+b)/2
    wfactor dx/dy = h/2 
   */

  template<typename FUN>
  double Quadrature::apply(FUN const & f)
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



#endif
