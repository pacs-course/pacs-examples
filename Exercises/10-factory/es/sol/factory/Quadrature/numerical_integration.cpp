#include "numerical_integration.hpp"
namespace NumericalIntegration{

  Quadrature::Quadrature(QuadratureRuleHandler const & rule, const Mesh1D & mesh):_rule(rule),_mesh(mesh){}


}
