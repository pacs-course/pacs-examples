#ifndef __NUMERICAL_INTEGRATION_HPP
#define __NUMERICAL_INTEGRATION_HPP
#include <vector>
#include "numerical_rule.hpp"
#include "mesh.hpp"
#include "wrapper.hpp"
/*!
  \file numerical_integration.hpp

  The previous organization is subject to various criticisms:

  - The class encapsulates too many concepts:
    -# The concept of composite integration
    -# The concept of mesh on which the discretization is carried out
    -# The handle can be made generic

*/
namespace NumericalIntegration{
  using namespace Geometry;

  typedef Wrapper<QuadratureRule> QuadratureRuleHandler;

  class Quadrature
  {
  public:
    /*!
      The constructur is very simple
      /par rule The rule to be adopted
      /par

     */
    Quadrature(const QuadratureRuleHandler & rule, const Mesh1D &);
    template<typename T>
    double apply(T const &) const;
  protected:
    QuadratureRuleHandler _rule;
    Mesh1D _mesh;
  };

  template <typename T>
   double Quadrature::apply(T const & f) const
   {
     double result(0);
     int nnodes=_rule->num_nodes();
     for(int i=0;i<_mesh.numNodes()-1;++i){
       double a=_mesh[i];
       double b=_mesh[i+1];
       double h2=(b-a)/2.0;
       double xm=(a+b)/2.0;
       result+=_rule->apply(f,xm,h2);
     }
     return result;
   }

  /*
    (-1,1) ->(a,b) y*(b-a)/2 + (a+b)/2
    wfactor dx/dy = h/2
   */



}



#endif
