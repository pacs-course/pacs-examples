#ifndef QUADRATURERULEADAPTIVE_HPP
#define QUADRATURERULEADAPTIVE_HPP
#include <cmath>
#include <stdexcept>
#include <queue>
#include <utility>
//#include <iostream>
#include "QuadratureRulePlusError.hpp"

namespace NumericalIntegration{

  /*!
    \brief Adaptive quadrature rules based on StandardQuadratureRule

    This class implements the Decorator design pattern.
    It is implemented in terms of a concrete StandardQuadrature rule.
   */
  
  template <class SQR>
  class QuadratureRuleAdaptive : public QuadratureRule
  {
  public:
    //! Constructor.
    /*!
      It takes compulsorily in input  a standard quad rule and information on the target error. Optionally
      you may set the total numer of iterations, which limits the number of subintervals used in the adaptive
      algorithm.
    */
    QuadratureRuleAdaptive(double targetError=1.e-6,unsigned int maxIter=100);
    //! Copy constructor.
    QuadratureRuleAdaptive(QuadratureRuleAdaptive const &)=default;
    
    //! The clone method.
    /*!  
      The override qualifier hels to avoid error (C++11): this
      method must override a virtual method of the base class.
     */
    QuadratureRuleHandler   clone() const override;
    //!
    /*!@{*/
    int num_nodes()const {return _therule.num_nodes();}
    double node(const int i) const {return _therule.node(i);};
    double weight(const int i)const{return _therule.weight(i);};
    unsigned int order() const{return _therule.order();};
    /*!@}*/
    //! set Target Error
    void setTargetError(double const t){_targetError=t;};
    //! set Max number of iterations
    void setMaxIter(unsigned int n){_maxIter=n;};
    //! The method that applies the rule.
    virtual double apply(FunPoint const &, double const & a,
			 double const & b) const override;
    virtual ~QuadratureRuleAdaptive(){};
  private:
    //! Static becouse common to all objects of this class
    static QuadratureRulePlusError<SQR> _therule;
    double _targetError;
     unsigned int _maxIter;
   };

  template <class SQR>
  QuadratureRulePlusError<SQR> QuadratureRuleAdaptive<SQR>::_therule;
  

  // Here a cannot use simply clone() since it returns a unique_ptr to a 
  // QuadratureRule and not StandardQuadratureRule, so I need to get the
  // raw pointer using release()
  template <class SQR>
  QuadratureRuleAdaptive<SQR>::QuadratureRuleAdaptive(double targetError, unsigned int maxIter):
   _targetError(targetError), _maxIter(maxIter)
  {}

  template <class SQR>
  QuadratureRuleHandler QuadratureRuleAdaptive<SQR>::clone() const
  { return  QuadratureRuleHandler(new QuadratureRuleAdaptive<SQR>(*this));}

  /*!
    @detail In the apply method I use the facilities of the QudaratureRUlePlusError classes
    to compute the error in each subinterval.
  */
  template<class SQR>
  double QuadratureRuleAdaptive<SQR>::apply(FunPoint const & f, double const & a,
					double const & b) const
  {
    using std::queue;
    using std::pair;
    using std::make_pair;
    unsigned int counter(0);
    double result(0);
    double dSize=b-a;
    queue<pair<double,double> > subint;
    subint.push(make_pair(a,b));

    while(counter<_maxIter && !subint.empty()){
      pair<double,double> z = subint.front();
      subint.pop();
      ExtractError::reset();
      double x1=z.first;
      double x2=z.second;
      double h2=(x2-x1);
      double errorLocalTarget=_targetError*h2/dSize;
      double lr=this->_therule.apply(f,x1,x2);
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
    //std::cout<<"Num Iter= "<<counter<<std::endl;    
	return result;
  }
}


#endif
