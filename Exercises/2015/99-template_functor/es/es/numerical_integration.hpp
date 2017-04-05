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
    double apply( FunPoint const & f);

  protected:

    const QuadratureRule _rule;
    Mesh1D _mesh;
  };

  /*
    (-1,1) ->(a,b) y*(b-a)/2 + (a+b)/2
    wfactor dx/dy = h/2 
   */
}



#endif
