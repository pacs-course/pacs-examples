#include <stack>
#include <utility>
#include "QuadratureRuleAdaptive.hpp"
namespace NumericalIntegration{

  // Here a cannot use simply clone() since it returns a unique_ptr to a 
  // QuadratureRule and not StandardQuadratureRule, so I need to get the
  // raw pointer using release()
  QuadratureRuleAdaptive::QuadratureRuleAdaptive(StandardQuadratureRule const & e, double targetError, unsigned int maxIter):
    _factor(1./(1-std::pow(2,1.0-e.order()))),
    _therule(static_cast<QuadratureRulePlusError *>(QuadratureRulePlusError(e).clone().release())),
    _targetError(targetError), _maxIter(maxIter)
  {}

  QuadratureRuleAdaptive::QuadratureRuleAdaptive(QuadratureRuleAdaptive const & rhs):
    _factor(rhs._factor),
    _therule(static_cast<QuadratureRulePlusError *>(rhs._therule->clone().release())),
    _targetError(rhs._targetError), _maxIter(rhs._maxIter)
  {}


  QuadratureRuleHandler QuadratureRuleAdaptive::clone() const
  { return  QuadratureRuleHandler(new QuadratureRuleAdaptive(*this));}

  /*!
    @detail In the apply method I use the facilities of the QudaratureRUlePlusError classes
    to compute the error in each subinterval.
  */
  double QuadratureRuleAdaptive::apply(FunPoint const & f, double const & a,
					double const & b) const
  {
    using std::stack;
    using std::pair;
    using std::make_pair;
    unsigned int counter(0);
    double result(0);
    double dSize=b-a;
    stack<pair<double,double> > subint;
    subint.push(make_pair(a,b));

    while(counter<_maxIter && !subint.empty()){
      pair<double,double> z = subint.top();
      subint.pop();
      ExtractError::reset();
      double x1=z.first;
      double x2=z.second;
      double h2=(x2-x1);
      double errorLocalTarget=_targetError*h2/dSize;
      double lr=_therule->apply(f,x1,x2);
      double localError=std::abs(ExtractError::error);
      ++counter;
      if (localError<=errorLocalTarget) result+=lr;
      else{
	subint.push(make_pair(x1,(x1+x2)/2));
	subint.push(make_pair((x1+x2)/2,x2));
	}
    }      
    if(counter>=_maxIter) 
      throw std::runtime_error("Max number iteration exceeded in QuadratureRuleAdaptive");
    return result;
  }
}



