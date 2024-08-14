#ifndef QUADRATURERULEPLUSERROR_HPP
#define QUADRATURERULEPLUSERROR_HPP
#include <cmath>

#include "QuadratureRuleBase.hpp"

namespace apsc::NumericalIntegration
{
/*!
  \brief Standard quadrature rule plus computation of error estimate.

  This class implements the Decorator design pattern:
  it is implemented in terms of an object of the
  StandardQuadratureRule hierarchy and at the same time
  it derives from the base StandardQuadratureRule class.

  It purpose is to enrich the apply() method to calculate
  an estimate of the error using the classical formula based
  on comparing the integral over h and that over h/2.

  Since the Quadrature rule is composed inside the class that
  implements composite integration, I need a way to
  gather the information about the value of the error estimate of
  the given rule. I have chosen to use an mutable variable of an embedded struct
 */

template <typename SQR>
class QuadratureRulePlusError final : public QuadratureRuleBase
{
public:
  QuadratureRulePlusError();
  //! Cloning method.
  std::unique_ptr<QuadratureRuleBase> clone() const override;
  //! Number of quadrature nodes. Delagates to the stored rules.
  constexpr unsigned int
  num_nodes() const
  {
    return therule_.num_nodes();
  }
  //! Coordinate of i-th node. Delegates to the stored rules.
  double
  node(const int i) const
  {
    return therule_.node(i);
  };
  //! Weight of i-th node. Delegates to the stored rules.
  double
  weight(const int i) const
  {
    return therule_.weight(i);
  };
  //! Order of convergence of the rule.
  unsigned int
  order() const
  {
    return therule_.order();
  };
  //! The factor used to compute the error estimate
  double
  factor() const
  {
    return factor_;
  }
  //! The actual rule.
  double apply(FunPoint const &, double const &a,
               double const &b) const override;
  //! String to identify the rule type
  std::string
  name() const override
  {
    return "QuadratureRulePlusError";
  }
  //! Internal class to store the error
  struct Error
  {
    double error = 0.0;
    void
    reset()
    {
      error = 0.0;
    }
  };
  //! The error.
  //! @note mutable since it may change even on const object. Indeed apply() is
  //! const since it override a const method of the base class.
  mutable Error ExtractError;

protected:
  //! The underlying rule. static because it is common to all objects of the
  //! class inlined so this is a definition (since C++17)
  inline static SQR therule_;
  //! Factor used in the calculation of the error estimate.
  double factor_;
};
// Not needed in C++17
// template<typename SQR>
// SQR QuadratureRulePlusError<SQR>::therule_;

template <typename SQR>
QuadratureRulePlusError<SQR>::QuadratureRulePlusError()
  : factor_(1. / (1 - std::pow(2, 1.0 - therule_.order())))
{}

template <typename SQR>
std::unique_ptr<QuadratureRuleBase>
QuadratureRulePlusError<SQR>::clone() const
{
  return std::make_unique<QuadratureRulePlusError<SQR> >(*this);
}

/*!
  In the apply method I am using the error formula in a strange way
  because want to replicate exactly the behavior of the
  QuadratureBase classes. Thus I compute the error of the formula
  with step h even if I have available the result with step
  h/2. Consistently, I return the value for h and not h/2.
*/
template <typename SQR>
double
QuadratureRulePlusError<SQR>::apply(FunPoint const &f, double const &a,
                                    double const &b) const
{
  double result = therule_.apply(f, a, b);
  double xm = (a + b) / 2.0;
  double result2 = therule_.apply(f, a, xm) + therule_.apply(f, xm, b);
  // Record the error in the extractor.
  ExtractError.error += (result2 - result) * factor_;
  return result; // I should return result2 see commment above
}
} // namespace apsc::NumericalIntegration

#endif
