#include "adaptive_quadrature.h"


double trapz::quad (std::function <double (double)> f, double a, double b)
{
  return (f(a) + f(b))* .5 * (b-a);
};

double midpoint::quad (std::function <double (double)> f, double a, double b)
{
  return (f((a+b)*.5) * (b-a));
};

factory::factory (std::string s)
{
  if (s == "trapz")
    ai = new trapz;
  else if (s == "midpoint")
    ai = new midpoint;
  else throw (std::out_of_range ("unexpected integrator name"));
};

double factory::quad (std::function <double (double)> f, double a, double b)
{
  return ai->quad (f, a, b);
};


double
adaptive_quadrature::refine (double a, double b, double oldval)
{ 
  ++depth;
  if (depth > depth_reached)
    depth_reached = depth;
  
  double c = .5 * a + .5 * b;
  
  double oldval_l = integrator.quad (f, a, c);
  double oldval_r = integrator.quad (f, c, b);
  double newval = oldval_l + oldval_r;
  
  double retval = 0.0;
  
  if (depth >= maxdepth
      || fabs (newval - oldval) <= tol)
    retval = newval;
  else
    {      
      retval += refine (a, c, oldval_l);
      retval += refine (c, b, oldval_r);
    }
  --depth;
  return retval;
};

 
double
adaptive_quadrature::integrate ( )
{
  double res = 0;
  depth = 0;
  depth_reached = 0;
  res = refine (start, finish, 0.0);
  /*std::cout << std::setprecision (16)
    << "res = " << res
    << " , depth_reached = "
    << depth_reached
    << std::endl;   */        
  return res;
};


