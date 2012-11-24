#include "numerical_integration.hpp"
namespace NumericalIntegration{
  
  Quadrature::Quadrature(QuadratureRuleHandler rule, Mesh1D const & mesh):_rule(std::move(rule)),_mesh(mesh){}

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
    for(unsigned int i=0;i<_mesh.numNodes()-1;++i){
      double a=_mesh[i];
      double b=_mesh[i+1];
      result += _rule->apply(f,a,b);
    }
    return result;
  }

}
