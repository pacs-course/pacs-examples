#ifndef QUADRATURERULEADAPTIVE_HPP
#define QUADRATURERULEADAPTIVE_HPP
#include <cmath>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <utility>

#include "QuadratureRulePlusError.hpp"

namespace apsc::NumericalIntegration
{
/*!
  \brief Adaptive quadrature rules based on StandardQuadratureRule

  This class implements the Decorator design pattern.
  It is implemented in terms of a concrete StandardQuadrature rule.
 */

template <class SQR>
class QuadratureRuleAdaptive final : public QuadratureRuleBase
{
public:
  //! Constructor.
  /*!  It in input the target error. Optionally you may set the
    total numer of iterations, which limits the number of
    subintervals used in the adaptive algorithm.
  */
  QuadratureRuleAdaptive(double       targetError = 1.e-6,
                         unsigned int maxIter = 100);

  //! The clone method.
  /*!
    The override qualifier helps to avoid error: this
    method must override a virtual method of the base class.
   */
  std::unique_ptr<QuadratureRuleBase> clone() const override;
  //!
  /*!@{*/
  constexpr unsigned int
  num_nodes()const
  {
    return _therule.num_nodes();
  }
  double
  node(const int i) const
  {
    return _therule.node(i);
  };
  double
  weight(const int i) const
  {
    return _therule.weight(i);
  };
  unsigned int
  order() const
  {
    return _therule.order();
  };
  /*!@}*/
  //! set Target Error
  void
  setTargetError(double const t) override
  {
    _targetError = t;
  };
  //! set Max number of iterations
  void
  setMaxIter(unsigned int n) override
  {
    _maxIter = n;
  };
  //! The method that applies the rule.
  double apply(FunPoint const &, double const &a,
               double const &b) const override;
  std::string
  name() const override
  {
    return "Adaptive";
  }

private:
  //! inline since since c++17 this way we have  a definition.
  QuadratureRulePlusError<SQR> _therule;
  double                       _targetError;
  unsigned int                 _maxIter;
};

// *** IMPLEMENTATIONS
template <class SQR>
QuadratureRuleAdaptive<SQR>::QuadratureRuleAdaptive(double       targetError,
                                                    unsigned int maxIter)
  : _targetError(targetError), _maxIter(maxIter)
{}

template <class SQR>
std::unique_ptr<QuadratureRuleBase>
QuadratureRuleAdaptive<SQR>::clone() const
{
  return std::unique_ptr<QuadratureRuleBase>(
    new QuadratureRuleAdaptive<SQR>(*this));
}

/*!
  @detail In the apply method I use the facilities of the
  QudaratureRUlePlusError classes to compute the error in each subinterval.
*/
template <class SQR>
double
QuadratureRuleAdaptive<SQR>::apply(FunPoint const &f, double const &a,
                                   double const &b) const
{
  using std::make_pair;
  using std::pair;
  using std::queue;
  unsigned int                 counter(0);
  double                       result(0);
  double                       dSize = b - a;
  queue<pair<double, double> > subint;
  subint.push(make_pair(a, b));

  while(counter < _maxIter && !subint.empty())
    {
      pair<double, double> z = subint.front();
      subint.pop();
      _therule.ExtractError.reset();
      double x1 = z.first;
      double x2 = z.second;
      double h2 = (x2 - x1);
      double errorLocalTarget = _targetError * h2 / dSize;
      double lr = this->_therule.apply(f, x1, x2);
      double localError = std::abs(_therule.ExtractError.error);
      ++counter;
      if(localError <= errorLocalTarget)
        result += lr;
      else
        {
          subint.push(make_pair(x1, (x1 + x2) / 2));
          subint.push(make_pair((x1 + x2) / 2, x2));
        }
    }
  // std::clog<<"Number of Iterations in Adaptive Rule="<<counter<<std::endl;
  if(counter >= _maxIter)
    // throw std::runtime_error("Max number iteration exceeded in
    // QuadratureRuleAdaptive");
    std::cerr << "Max number iteration exceeded in QuadratureRuleAdaptive: "
              << counter << std::endl;
  return result;
}
} // namespace apsc::NumericalIntegration

#endif
