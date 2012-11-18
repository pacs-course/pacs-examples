#ifndef QUADRATURE_RULE_HPP
#define QUADRATURE_RULE_HPP
#include <vector>
#include <memory>
#include <utility>
namespace NumericalIntegration{
  
  //! The type the integrand
  typedef std::function<double (double const &)> FunPoint;

  
  //! The basis class for all the basic integration rules
  /*
    This basis class is the common class for all numerical integration 
    formulae that approximate the integral \f$ \int_{-1}^{1} f(y) dy\f$.
    Its a very light class (no variable members), that provides
    the common interface to all integration rules.

    Constructors and assignement operators
    are not defined since the synthetic ones are sufficient.


  */
  class QuadratureRule
  {
  public:
    //! The class is clonable.
    /*!
      Having a clonable class makes it possible to write copy constructors
      and assignment operators for classes that aggregate object of the
      QuadratureRule hierarchy by composition.
    */
    virtual std::unique_ptr<QuadratureRule> clone() const =0;
    // Applies the rule in the interval (-1,1)
    virtual double apply(FunPoint const & f)const=0;
  };
  
  /*! \brief  A base class for standard quadrature rules.
    
    This class provides the common interface to  a quadrature rule in
    the interval \f$[-1,1]\f$ computed by the formula

    \f$ \int_{-1}^{1} f(y) dy \simeq  \sum_{i=0}^{m-1}w_i f(y_i)\f$

    It also provides the containers for nodes \f$y_i\f$ and weights
    \f$w_i\f$.  This is the base class, the derived class constructs
    the appropriate points and weights. No method is overritten in the
    derived class (a part the destructor and the clone method).

    Constructors and assignement operators
    are not defined since the synthetic ones are sufficient.

    This version implements the virtual constuctor paradigm through the
    method clone();
  */

  //! The type of the object holding the quadrature rule.
  typedef std::unique_ptr<QuadratureRule> QuadratureRuleHandler;

  class StandardQuadratureRule: public QuadratureRule
  {
  public:
    //! number of nodes used by the rule
    inline int num_nodes() const;
    //! access to the i-th node
    inline double node(const int i) const;
    //! access to the i-th weight
    inline double weight(const int i)const;
    //! The order of convergence
    inline unsigned int order()const;
    //! The class is clonable.
    /*!
      Having a clonable class makes it possible to write copyconstructors
      and assignment operators for classes that aggregate object of the
      QuadratureRule hierarchy by composition.
     */
    virtual QuadratureRuleHandler clone() const =0;
    // Applies the rule in the interval (-1,1)
    double apply(FunPoint const & f)const;
  protected:
    std::vector<double>  _w;
    std::vector<double>  _n;
    unsigned int my_order;
  private:
  };


  // Definition of inlined methods

  inline int StandardQuadratureRule::num_nodes() const
  { return _w.size();}

  inline double StandardQuadratureRule::node(const int i) const
  { return _n[i];}
  
  inline double StandardQuadratureRule::weight(const int i) const
  { return _w[i];}
  
  inline unsigned int StandardQuadratureRule::order() const
  { return my_order;}
}
#endif
