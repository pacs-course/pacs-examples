#ifndef _NUMERICAL_INTEGRATION_HPP
#define _NUMERICAL_INTEGRATION_HPP 1
#include <vector>
#include "numerical_rule.hpp"

/*!
  \file numerical_integration.hpp
  \brief delegating the rule over a single interval

  The previous organization is subject to various criticisms:

  -# The class encapsulates too many concepts:
  -# The concept of composite integration rule
  -# The quadrature rule itself, which is normally defined in
     the interval \f$[-1,1]\f$.

   - Very similar code is repeated in the apply() methods.

   Here instead, the quadrature rule on the interval \f$[-1,1]\f$ is handled
   by a specific hierarchy of classes (whose declaration is contained in
   the file numerical_rule.hpp) while the class Quadrature takes care of
   implementing the rule in a composite context:

   \f[
      I_h=\sum_{i=0}^{N_{int}-1} \frac{h}{2}\sum_{j=0}^{N_{nodes}-1}
          \alpha_j f(\overline{x}_i+\frac{hy_j}{2})
   \f]

   Note that the constructor now takes a reference to the quadrature rule
   on the interval \f$[-1,1]\f$.

*/
namespace NumericalIntegration
{

  typedef double (*FunPoint)(double const &);

  class Quadrature
  {

  public:

    /*!
      \brief The constructor

      \param rule The rule in the reference interval \f$[-1,1]\f$
      \param a The left end
      \param b the right end
      \param Nint the number of intervals

     */
    Quadrature ( const QuadratureRule & rule,
                 double const & a, double const & b,
                 unsigned int const & Nint);

    //! It applies the composite rule
    double apply ( const FunPoint& f ) const;

  protected:

    const QuadratureRule& M_rule;

    double M_a, M_b, M_h;

    //! We store the change of measure \f$ h/2\f$
    double M_wf;

    int M_nint;

    std::vector<double> M_knots;

  };

} // namespace NumericalIntegration
#endif
