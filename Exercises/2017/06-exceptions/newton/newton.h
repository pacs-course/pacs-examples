#include <iostream>
#include <limits>
#include <cmath>
#include <functional>
#include "ad.h"

class
newton_solver
{
 private:

  var fun;

  int    maxit;
  double funtol;
  double xtol;
  double x;
  double dfdx;
  double dx;
  double damp_factor;
  double damp_min;
  double damp;
  double r;
  int    iter;


  struct
  increment_failed_t : public std::exception
  {
    virtual const char *what () const throw ()      
    { return "residual is not decreasing"; };
  } increment_failed;

  struct
  small_damping_t : public std::exception
  {
    virtual const char *what () const throw ()      
    {
      const char *s = "damping parameter too small";
      std::cerr << s << std::endl;
      return s;
    };
  } small_damping;
  
  double
  compute_increment ()
  {

    if (damp <= damp_min)
      throw (small_damping);
    
    double dx = 0.0;
    dx   = - r / dfdx;
    double newres = std::abs (fun.eval (x + damp * dx));
    if (newres >= std::abs (r))
      throw (increment_failed);       
    return (damp * dx);
  };
  
 public:
  
  newton_solver (const var &fun_,
                 const int maxit_ = 100,
                 const double funtol_ = std::numeric_limits<double>::epsilon () * 1000.0,
                 const double xtol_ = std::numeric_limits<double>::epsilon () * 1000.0,
                 const double damp_factor_ = 2.0,
                 const double damp_min_ = std::numeric_limits<double>::epsilon () * 10000.0)
    : fun (fun_), maxit (maxit_), funtol (funtol_), xtol (xtol_),   damp_factor (damp_factor_), damp_min (damp_min_)
  { x = 0; dfdx = 0; r = 0; iter = 0; }
    

  void
  solve (const double x0)
  {
    x = x0;
    for (iter = 0; iter < maxit; ++iter)
      {        
        damp = 1.0;
        r    = fun.eval (x);
        if (std::abs (r) < funtol) break;
                
        dfdx = fun.eval_der (x);

        std::cout << "x = " << x << " r = " << r << " dfdx = " << dfdx << std::endl;
                  
        bool good_step = false;
        while (! good_step)
          {
            try
              {
                dx = compute_increment ();
                good_step = true;
              }
            catch (increment_failed_t &t)
              {
                std::cout << "caught exception : " << t.what () << std::endl;
                good_step = false;
                damp /= damp_factor;
                std::cout << "damp = " << damp << std::endl;
              }
          }
        if (std::abs (dx) < xtol)  break;
        
        x += dx;
      }

  }

  double
  get_result ()
  { return x; };

  double
  get_residual ()
  { return r; };

  int
  get_iter ()
  { return iter; };

};
