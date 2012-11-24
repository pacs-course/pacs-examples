#ifndef QUADRATURERULEPLUSERROR_HPP
#define QUADRATURERULEPLUSERROR_HPP
#include <cmath>
#include "QuadratureRule.hpp"

namespace NumericalIntegration{

  //! A class that will record the error.
  /*
    Since the Quadrature rule is composed inside the class that
    implements composite integration, I need an external object to
    gather the information about the value of the error estimate of
    the given rule. I have chosen to use static members, but other
    solutions ar possible, for instance storing inside the
    QuadratureRulePlusError class a reference to an object whose role
    is to record the error. This latter solution is also safer, I have
    preferred the simpler one in this example.
   */
  struct ExtractError
  {
    static double error;
    static void reset();
  };
  
  /*!
    \brief Standard quadrature rule plus error.

    This class implements the Decorator design pattern:
    it is implemented in terms of an object of the
    StandardQuadratureRule hierarchy and at the same time
    it derives from the base StandardQuadratureRule class.

    It purpose is to enrich the apply() method to calculate
    an estimate of the error using the classical formula based
    on comparing the integral over h and that over h/2.

    It uses the ExtractError class to pass out the information
    about the error.
   */

  class QuadratureRulePlusError: public StandardQuadratureRule
  {
  public:
    //! Constructor takes in input a standard quadrature rule.
    QuadratureRulePlusError(StandardQuadratureRule const &);
    //! This constructor moves the rule.
    QuadratureRulePlusError(StandardQuadratureRule &&);
    //! Copy constructor.
    QuadratureRulePlusError(QuadratureRulePlusError const &);
    //! Cloning method.
    virtual QuadratureRuleHandler   clone() const;
    //! Number of quadrature nodes. Delagates to the stored rules.
    int num_nodes()const {return _therule->num_nodes();}
    //! Coordinate of i-th node. Delegates to the stored rules.
    double node(const int i) const {return _therule->node(i);};
    //! Weight of i-th node. Delegates to the stored rules.
    double weight(const int i)const{return _therule->weight(i);};
    //! Order of convergence of the rule.
    unsigned int order() const{return _therule->order();};
    //! The actual rule.
    virtual double apply(FunPoint const &, double const & a,
			 double const & b) const;
  private:
    //! Factor used in the calculation of the error estimate.
    double _factor;
    //! The underlying rule.
    std::unique_ptr<StandardQuadratureRule> _therule;
  };
}


#endif
