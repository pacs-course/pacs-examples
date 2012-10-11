#ifndef __NUMERICAL_RULE_HPP
#define __NUMERICAL_RULE_HPP
#include <vector>
namespace NumericalIntegration{
  
  /*!  This class takes care of the data associated to a quadrature rule in
    the interval \f$[-1,1]\f$. It stores the points and the weights.

    The public interface is defined by the base class, the derived class
    construct the appropriate data. No method is overritten in the derived
    class (a part the desctructor, which must be virtual).

    The use with a wrapper requires the object of this class hierarchy to
    be copy contructable. The automatic copy constructor works fine
    here.
    
    When you need to specify a destructor in general you need also a copy
    constructor and the assignement operator.
    
    This version implements the virtual constuctor paradigm through the
    method clone();
  */
  
  class QuadratureRule
  {
  public:

    // Constructors and destructors.
    //! Constructors and destructors.
    //@{

    //! Costructor that fill the parameters.
    QuadratureRule();

    //! Virtual destructor.
    virtual ~QuadratureRule();

    //@}

    // Get methods
    //! Get methods
    //@{

    /*!
      Return the number of nodes used by the rule.
      @return The number of nodes.
    */
    inline int num_nodes() const;

    /*!
      Return the access to the i-th node.
      @param i The index of the node.
      @return The i-node.
    */
    inline double node( const int& i ) const;

    /*!
      Return the access to the i-th weight.
      @param i The index of the weight.
      @return The i-weight.
    */
    inline double weight( const int& i ) const;

    /*!
      Return the order of convergence.
      @return The order of convergence.
    */
    inline unsigned int order() const;

    //@}

    virtual QuadratureRule * clone() const =0;

  protected:

    // Data values for the class.
    //! @name Data values for the class.
    //@{

    //! Vecotr of weights.
    std::vector<double>  M_w;

    //! Vector of nodes.
    std::vector<double>  M_n;

    //! Order of convergence.
    unsigned int         M_order;

    //@}

  };
  
  /*!
    @class Simpson

    This class implement the Simpson quadrature rule.
  */
  class Simpson : public QuadratureRule
  {

  public:

    // Constructors and destructors.
    //! Constructors and destructors.
    //@{

    //! Costructor that fill the parameters.
    Simpson();

    //@

    virtual QuadratureRule * clone() const;

  };

  /*!
    @class MidPoint

    This class implement the midpoint quadrature rule.
  */
  class MidPoint : public QuadratureRule
  {

  public:

    // Constructors and destructors.
    //! Constructors and destructors.
    //@{

    //! Costructor that fill the parameters.
    MidPoint();

    //@}

    virtual QuadratureRule * clone()const ;
  };

  /*!
    @class Trapezoidal

    This class implement the trapezoidal quadrature rule with four points.
  */
  class Trapezoidal : public QuadratureRule
  {
  public:

    // Constructors and destructors.
    //! Constructors and destructors.
    //@{

    //! Costructor that fill the parameters.
    Trapezoidal();

    //@}

    virtual QuadratureRule * clone()const ;

  };

  /*!
    @class Gauss4Pt

    This class implement the gaussian quadrature rule with four points.
  */
  class Gauss4Pt : public QuadratureRule
  {

  public:

    // Constructors and destructors.
    //! Constructors and destructors.
    //@{

    //! Costructor that fill the parameters.

    Gauss4Pt();

    //@}

    virtual QuadratureRule * clone()const ;

  };

  // Return the number of nodes
  int QuadratureRule::num_nodes() const
  { 
    return M_w.size();
  }// num_nodes

  // Return the i-node.
  double QuadratureRule::node(const int& i) const
  { 
    return M_n[i];
  }// node

  // Return the i-weight.
  double QuadratureRule::weight(const int& i) const
  { 
    return M_w[i];
  }// weight

  // Return the order of the quadrature rule.
  unsigned int QuadratureRule::order() const
  { 
    return M_order;
  }// order

}

#endif
