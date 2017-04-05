/*!
  \file numerical_rule.hpp
  \brief A collection of classes describing quadrature rules
  \author Luca Formaggia
*/

#ifndef __NUMERICAL_RULE_HPP
#define __NUMERICAL_RULE_HPP 1

#include <vector>

namespace NumericalIntegration{

  /*!
    \class QuadratureRule
    \brief Approximates the integral of a function f(x)
    over the interval (-1,1)
  */
  class QuadratureRule
  {
  public:
    /** @name Constructors
     */
    //@{
    //! Default constructor
    QuadratureRule();
    //@}
    /** @name Accessors
     */
    //@{
    //! Gets the number of nodes
    int num_nodes() const;
    //! Gets the coordinate of i-th node
    double node(const int i) const;
    //! Gets th weigth for i-th node
    double weight(const int i)const;
    //@}
    /** @name Destructors
     */
    //@{
    //! virtual destructor
    virtual ~QuadratureRule();
    //@}
  protected:
    std::vector<double>  _w; /* weigths */
    std::vector<double>  _n; /* nodes */
  };
  
    
  /*!
    \class Simpson
    \brief Approximates the integral of a function f(x)
    over the interval (-1,1), using Simpson rule
  */
  class Simpson : public QuadratureRule
  {
  public:
    /** @name Constructors
     */
    //@{
    //! Default constructor
    Simpson();
    //@}
  };


  /*!
    \class MidPoint
    \brief Approximates the integral of a function f(x)
    over the interval (-1,1), using mid point rule
  */
  class MidPoint : public QuadratureRule
  {
  public:
    /** @name Constructors
     */
    //@{
    //! Default constructor
    MidPoint();
    //@}
  };


}

#endif
