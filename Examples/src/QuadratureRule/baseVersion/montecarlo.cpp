#include<cmath>
#include <iostream>
#include <random>
#include "montecarlo.hpp"
namespace NumericalIntegration{

  MonteCarlo::MonteCarlo(): max_iter(max_iter_def),
			    error_level(error_level_def),M_lastError(0),
			    M_cumulatedError(0),M_iterationsExceeded(false)
  {}

  std::unique_ptr<QuadratureRule> MonteCarlo::clone() const { return std::unique_ptr<QuadratureRule>(new MonteCarlo(*this));}

  void MonteCarlo::setError(double e){error_level=e;}
  void MonteCarlo::setMiter(unsigned int  m){max_iter=m;}
  
  double MonteCarlo::apply(FunPoint const & f, double const & a,
			   double const & b) const
  {
    using namespace std;
    static constexpr unsigned int min_iter=16;  
    random_device rd;
    // initialize random engine with the
    // random device
    knuth_b rengine(rd());
    uniform_real_distribution<> dis(a, b);
    unsigned int  n(0);
    double mean(0);
    double S(0);
    double errore(1e+30);
    while ((n < max_iter && errore > error_level) || n<=min_iter){
      ++n;
      double x=dis(rengine);
      double y=f(x);
      double delta = y - mean;
      // Adjourn mean
      mean += delta/n;
      // Adjourn s.dev.
      S +=  delta*(y - mean);
      // Adjourn error estimate
      errore=(b-a)*sqrt(S)/n;
    }
    M_lastError=errore;
    M_cumulatedError+=errore;
    M_iterationsExceeded=(n>=max_iter);
    if(M_iterationsExceeded) 
      std::clog<<"Montecarlo N. iterations exceeded"<<" Error "<<
	errore<<" Target error "<< error_level<<std::endl;
    return (b-a)*mean;
  }

  void MonteCarlo::resetError()
  {
    M_cumulatedError=0;
    M_iterationsExceeded=false;
  }
  
  
}
