#include "numerical_integration.hpp"
namespace NumericalIntegration{
  
  Quadrature::Quadrature(QuadratureRuleHandler rule, Mesh1D & mesh):_rule(std::move(rule)),_mesh(mesh){}

  /*! 
    Formally identical! I need a move-constructor in the mesh class to have 
    a different behaviour.
  */

  Quadrature::Quadrature(QuadratureRuleHandler rule, Mesh1D && mesh):_rule(std::move(rule)),_mesh(std::move(mesh)){}



  //! I need to copy the QuadratureRule not its handler!
  Quadrature::Quadrature(Quadrature const & rhs):
    _rule(rhs._rule.get()),  _mesh(rhs._mesh){}
  
//! I need to copy the QuadratureRule not its handler!
  Quadrature & Quadrature::operator=(Quadrature const & rhs){
    if(this!=&rhs){
      _mesh=rhs._mesh;
      // Resets the current value replacing with the pointer
      // returned by rhs._rule
      _rule.reset(rhs._rule.get());
    }
    return *this;
  }

  double Quadrature::apply(FunPoint const & f) const
  {
    double result(0);
    // h2 half interval length
    // xm interval mid point
    double h2,xm;
    // scale fun from (-1 1) to given interval
    auto fscaled=[&](double x){return f(x*h2+xm);};
    for(unsigned int i=0;i<_mesh.numNodes()-1;++i){
      double a=_mesh[i];
      double b=_mesh[i+1];
      h2=(b-a)*0.5;
      xm=(a+b)*0.5;
      result += h2*_rule->apply(fscaled);
    }
    return result;
  }

}
