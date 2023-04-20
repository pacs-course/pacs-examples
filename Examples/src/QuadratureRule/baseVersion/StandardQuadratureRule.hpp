#ifndef QUADRATURE_RULE_HPP
#define QUADRATURE_RULE_HPP
#include <array>
#include <utility>

#include "QuadratureRuleBase.hpp"
namespace apsc::NumericalIntegration
{
/*! \brief  A base class for standard quadrature rules.

  This class provides the common interface to  a quadrature rule in
  the interval \f$[-1,1]\f$ computed by the formula

  \f$ \int_{-1}^{1} f(y) dy \simeq  \sum_{i=0}^{m-1}w_i f(y_i)\f$

  It also provides the containers for nodes \f$y_i\in[-1,1]\f$ and
  corresponding weights \f$w_i\f$.  This is the base class, the
  derived class constructs the appropriate points and weights. No
  method is overritten in the derived class (a part the destructor
  and the clone method).

  Constructors and assignement operators
  are not defined since the synthetic ones are sufficient.

  This version implements the virtual constuctor paradigm through the
  method clone();
*/
template <unsigned int NKNOTS>
class StandardQuadratureRule : public QuadratureRuleBase
{
public:
  //! constructor
  /*!
    All quantities in the interval [-1,1]
    @param weight Weights of the rules
    @param nodes  Nodes (knots) of the rule
    @param order Order of the quadrature (exactness +1). Required only if
    adaptive rule is used
   */
  StandardQuadratureRule(std::array<double, NKNOTS> const & weight,
                         std::array<double, NKNOTS> const & nodes, double order = 0)
    : w_(weight), n_(nodes), my_order(order)
  {}
  //! class is default constructible
  StandardQuadratureRule() = default;
  //! number of nodes used by the rule
  constexpr unsigned int
  num_nodes() const
  {
    return NKNOTS;
  }
  //! access to the i-th node
  double
  node(const unsigned int i) const
  {
    return n_[i];
  }
  //! access to the i-th weight
  double
  weight(const unsigned int i) const
  {
    return w_[i];
  }
  /*!
   * The order of convergence
   * @note Used for error estimation
   * @return the order
   */
  unsigned int
  order() const
  {
    return my_order;
  }
  //! returns a string that identify the general type of quadrature rule
  std::string
  name() const override
    {
      return "Standard Quadrature Rule";
    };
  //! The class is clonable.
  /*!
    Having a clonable class makes it possible to write copyconstructors
    and assignment operators for classes that aggregate object of the
    QuadratureRule hierarchy by composition.
   */
  virtual std::unique_ptr<QuadratureRuleBase> clone() const override = 0;
  // Applies the rule in the interval (a,b)
  double apply(FunPoint const &f, double const &a,
               double const &b) const override;
  virtual ~StandardQuadratureRule() = default;

protected:
  std::array<double, NKNOTS> w_;
  std::array<double, NKNOTS> n_;
  unsigned int               my_order = 0;

private:
};

// *******************  IMPLEMENTATIONS
template <unsigned int N>
double
apsc::NumericalIntegration::StandardQuadratureRule<N>::apply(
  FunPoint const &f, double const &a, double const &b) const
{
  double h2=(b - a) * 0.5; // half length
  double xm=(a + b) * 0.5; // midpoint
  // scale functions
  auto   fscaled = [&h2,&xm,&f](double x) { return f(x * h2 + xm); };
  double tmp=0.0;
  auto   np = n_.begin();
  //    for (auto wp=w_.begin();wp<w_.end();++wp,++np)
  for(auto weight : w_)
      {
        tmp += fscaled(*(np)) * weight;
        ++np;
      }

  return h2 * tmp;
}

} // namespace apsc::NumericalIntegration
#endif
