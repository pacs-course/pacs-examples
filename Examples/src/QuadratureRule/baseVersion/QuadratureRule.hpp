#ifndef QUADRATURE_RULE_HPP
#define QUADRATURE_RULE_HPP
#include <array>
#include <memory>
#include <utility>
namespace NumericalIntegration{
  
  //! The type the integrand
  typedef std::function<double (double const &)> FunPoint;
  
  
  //! The basis class for all the basic integration rules
  /*
    This basis class is the common class for all numerical integration 
    formulae that approximate the integral \f$ \int_{-1}^{1} f(y) dy\f$.
    It's a very light class (no variable members) that provides
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
    // Applies the rule in the interval (a,b)
    
    virtual double apply(FunPoint const & f, double const &a, 
			 double const & b)const=0;
    virtual ~QuadratureRule()=default;
  };
  

  //! The type of the object holding the quadrature rule.
  using QuadratureRuleHandler=std::unique_ptr<QuadratureRule>;

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
  template<unsigned int NKNOTS>
  class StandardQuadratureRule: public QuadratureRule
  {
  public:
    //! constructor
    /*!
      All quantities in the interval [-1,1]
      @param weight Weights of the rules
      @param nodes  Nodes (knots) of the rule
      @param order Order of the quadrature (exactness +1). Required only if adaptive rule is used
     */
    StandardQuadratureRule(std::array<double,NKNOTS> weight,std::array<double,NKNOTS> nodes,double order=0):
      _w(weight),_n(nodes),my_order(order)
    {
    }
    //! Default constructor
    StandardQuadratureRule():_w(),_n(),my_order(0){}
    //! number of nodes used by the rule
    constexpr unsigned int num_nodes() const
    {
      return NKNOTS;
    }
    //! access to the i-th node
    double node(const unsigned int i) const
    {
      return _n[i];
    }
    //! access to the i-th weight
    inline double weight(const unsigned int i)const
    {
      return _w[i];
    }
    //! The order of convergence
    inline unsigned int order()const
    {
      return my_order;
    }
    //! The class is clonable.
    /*!
      Having a clonable class makes it possible to write copyconstructors
      and assignment operators for classes that aggregate object of the
      QuadratureRule hierarchy by composition.
     */
    virtual QuadratureRuleHandler clone() const =0;
    // Applies the rule in the interval (a,b)
    double apply(FunPoint const & f, double const &a, double const& b)const;
    virtual ~StandardQuadratureRule()=default;

  protected:
    std::array<double,NKNOTS>  _w;
    std::array<double,NKNOTS>  _n;
    unsigned int my_order;
  private:
  };
  
  template<unsigned int N>
  double 
  NumericalIntegration::StandardQuadratureRule<N>::apply(
							 FunPoint const &f, 
							 double const &a, 
							 double const &b)const
  {
    double h2((b-a)*0.5); // half length
    double xm((a+b)*0.5); // midpoint
    auto fscaled=[&](double x){return f(x*h2+xm);};
    double tmp(0);
    auto np=_n.begin();
    for (auto wp=_w.begin();wp<_w.end();++wp,++np)tmp+=fscaled(*np)*(*wp);
    return h2*tmp;
  }

}
#endif
