/*!
  \file numerical_integration.hpp
  \brief Implement a numerical integrator 
  \author Luca Formaggia
*/

#ifndef __NUMERICAL_INTEGRATION_HPP
#define __NUMERICAL_INTEGRATION_HPP 1

#include <vector>

#include "numerical_rule.hpp"

namespace NumericalIntegration{

  //! Type for function pointer (could use functors instead)
  typedef double (*FunPoint)(double const &);

  /*!
    \class Quadrature
    \brief A numerical integrator for f(x) in (a,b)
  */
  template<typename QR>
  class Quadrature
  {
  public:
    /** @name Constructors
     */
    //@{
    //! Constructor
    /*!
      \param a left boundary
      \param b right boundary
      \param Nint number of points inside (a, b)
    */
    Quadrature(double const & a, double const & b, int const & Nint);
    //@}
    /** @name Methods
     */
    //@{
    //! Find the approximated integral of f(x)
    /*!
      \param f function pointer
      \return integral over (a,b)
    */
    double apply(FunPoint f);
    //@}
  protected:
    QR _rule;
    double _a,_b,_h;
    double _wf;
    int _nint;
    std::vector<double> _knots;
  };


  
  template<typename QR>
  Quadrature<QR>::Quadrature(double const & a,
			 double const & b,
			 int const & nint):
    _rule(),_a(a),_b(b),_h((b-a)/(double) nint),_wf(_h/2.0),
    _nint(nint),_knots(nint+1)
  {
    _knots[0]=_a;
    for(int i=1;i<_knots.size();++i)_knots[i]=_knots[i-1]+_h;
  }
  
  template<typename QR>
  double Quadrature<QR>::apply(FunPoint f)
  {
    double result(0);
    int nnodes=_rule.num_nodes();
    for(int i=0;i<_knots.size()-1;++i){
      double a=_knots[i];
      double b=_knots[i+1];
      double xm=(a+b)/2.0;
      for (int j=0;j<nnodes;++j){
	double x=xm+_wf*_rule.node(j);
	result+= _wf*f(x)*_rule.weight(j);
      }
    }
    return result;
  }
  
}



#endif
