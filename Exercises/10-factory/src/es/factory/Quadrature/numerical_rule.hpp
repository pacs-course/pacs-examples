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
    QuadratureRule();
    int num_nodes() const;
    double node(const int i) const;
    double weight(const int i)const;
    unsigned int order()const;
    virtual QuadratureRule * clone() const =0;
    template <typename T>
    double apply(T const &, double const & shift,
				double const & scale) const;
  protected:
    std::vector<double>  _w; //pesi regola su [-1,1]
    std::vector<double>  _n; //nodi regola su [-1,1]
    unsigned int my_order;  // ordine della formula
  private:
  };

  template<typename T>
  double QuadratureRule::apply(T const & f, double const & shift,
				double const & scale) const
  {
    double result(0);
    for (int i=0; i<_w.size();++i) result+= _w[i]*f(_n[i]*scale+shift);
    return scale*result;
  }
}

#endif
