#include "numerical_integration.hpp"
namespace NumericalIntegration{
  
  
// I need to copy the QuadratureRule not its handler!
  Quadrature & Quadrature::operator=(Quadrature const & rhs){
    if(this!=&rhs){
      _mesh=rhs._mesh;
      // Resets the current value replacing with the pointer
      // returned by rhs._rule
      _rule=std::move(rhs._rule->clone());
    }
    return *this;
  }

// I need to copy the QuadratureRule not its handler!
  Quadrature & Quadrature::operator=(Quadrature&& rhs){
    if(this!=&rhs){
      _mesh=std::move(rhs._mesh);
      _rule=std::move(rhs._rule);
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
