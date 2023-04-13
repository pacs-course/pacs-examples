#include "numerical_integration.hpp"
namespace apsc::NumericalIntegration
{
// I need to copy the QuadratureRule not its handler!
/*Quadrature & Quadrature::operator=(Quadrature const & rhs){
  if(this!=&rhs){
    mesh_=rhs.mesh_;
    // Resets the current value replacing with the pointer
    // returned by rhs._rule
    rule_=rhs.rule_->clone();
  }
  return *this;
}
*/

double
Quadrature::apply(FunPoint const &f) const
{
  double result(0);
#ifdef _OPENMP
#pragma omp parallel for reduction (+:result)
#endif
  for(unsigned int i = 0u; i < mesh_.numNodes() - 1; ++i)
    {
      double a = mesh_[i];
      double b = mesh_[i + 1];
      result += rule_->apply(f, a, b);
    }
  return result;
}

} // namespace apsc::NumericalIntegration
