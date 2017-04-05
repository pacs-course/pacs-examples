#ifndef __NUMERICAL_RULE_HPP
#define __NUMERICAL_RULE_HPP
#include <vector>

namespace NumericalIntegration
{

  /*!
    @class QuadratureRule

    This class takes care of the data associated to a quadrature rule in
    the interval \f$[-1,1]\f$. It stores the points and the weights.
    <br>
    The public interface is defined by the base class, the derived class
    construct the appropriate data. No method is overwritten in the derived
    class (a part the destructor). Here the destructor does not need to be
    virtual since the base class owns all the data. Yet for safety I will
    make it virtual.
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

  private:

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

    //@}

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

  };

  // Return the number of nodes
  int QuadratureRule::num_nodes() const
  {
    return M_w.size();
  } // num_nodes

  // Return the i-node.
  double QuadratureRule::node( const int& i ) const
  {
    return M_n[i];
  } // node

  // Return the i-weight.
  double QuadratureRule::weight( const int& i ) const
  {
    return M_w[i];
  } // weight

  // Return the order of the quadrature rule.
  unsigned int QuadratureRule::order() const
  {
    return M_order;
  } // order

}
#endif
