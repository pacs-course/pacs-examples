#ifndef _NUMERICAL_RULE_HPP
#define _NUMERICAL_RULE_HPP 1

#include <vector>

namespace NumericalIntegration
{

  /*!
    This class takes care of the data associated to a quadrature rule in
    the interval \f$[-1,1]\f$. It stores the points and the weights.

    The public interface is defined by the base class, the derived class
    construct the appropriate data. No method is overwritten in the derived
    class (a part the destructor). Here the destructor does not need to be
    virtual since the base class owns all the data. Yet for safety I will
    make it virtual.

  */
  class QuadratureRule
  {
  public:

    QuadratureRule();

    virtual ~QuadratureRule();

    //! number of nodes used by the rule
    unsigned int num_nodes() const { return M_w.size(); }

    //! access to the i-th node
    double node ( const int i ) const { return M_n[i]; }

    //! access to the i-th weight
    double weight ( const int i ) const { return M_w[i]; }

    //! The degree of exactness
    unsigned int degree() const { return M_exactDegree; }

  protected:

    std::vector<double>  M_w;
    std::vector<double>  M_n;
    unsigned int M_exactDegree;

  private:

  };

  class Simpson : public QuadratureRule
  {
  public:
    Simpson();
  };

  class MidPoint : public QuadratureRule
  {
  public:
    MidPoint();
  };

} // namespace NumericalIntegration
#endif
