#ifndef HAVE_ADAPTIVE_QUADRATURE_H
#define HAVE_ADAPTIVE_QUADRATURE_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <functional>

class
abstract_integrator
{
public :
  virtual double quad (std::function <double (double)>, double, double) = 0;
};

class
trapz : public abstract_integrator
{
  double quad (std::function <double (double)>, double, double);
};

class
midpoint : public abstract_integrator
{
  double quad (std::function <double (double)>, double, double);
};

class
factory
{
private :

  abstract_integrator *ai;
  
public :

  factory (std::string s);
  double quad (std::function <double (double)>, double, double);
  
};

class
adaptive_quadrature
{
private :

  int depth = 0;
  int depth_reached = 0;

  std::function<double (double)> f;
  const double start, finish;
  const double tol;
  const int maxdepth;

  double
  refine (double a, double b, double oldval);

  factory integrator;

public :

  adaptive_quadrature (std::function<double (double)> f_, double start_,
                       double finish_, double tol_ = 1.0e-12, int maxdepth_ = 40,
                       std::string integrator_name_ = "trapz")
    : f(f_), start (start_), finish (finish_), tol (tol_), maxdepth (maxdepth_),
      integrator (integrator_name_)
  {};
  
  double
  integrate ();

};

#endif
