#ifndef ITERATIVEMETHOD_HPP
#define ITERATIVEMETHOD_HPP 1

#include <cassert>
#include <cmath>

///////////////////////////////////
// Class declarations

/// Base class template for iterative methods
template <typename real>
class IterativeMethod 
{
public:
  
  typedef real (*fctptr) (real);

  enum checkT {
    INCREMENT,
    RESIDUAL  
  };

  IterativeMethod(real tol, int maxit, checkT check);
  
  inline real& tol() { return M_tol; }
  inline int& maxit() { return M_maxit; }
  inline int nit() const { return M_nit; }
  
protected:
  
  bool converged (real, real);

  int M_nit;
  real M_tol;
  int M_maxit;
  checkT M_check;

};

/// Derived class template implementing the bisection method
template <typename real>
class Bisection : public IterativeMethod<real> {

public:

  typedef typename IterativeMethod<real>::checkT checkT;
  typedef typename IterativeMethod<real>::fctptr fctptr;

  Bisection (real tol, int maxit, checkT check);
  real apply (real, real, fctptr);

};

/// Derived class template implementing the Newton method
template <typename real>
class Newton : public IterativeMethod<real> {
public:

  typedef typename IterativeMethod<real>::checkT checkT;
  typedef typename IterativeMethod<real>::fctptr fctptr;

  Newton (real, int);
  real apply (real, fctptr, fctptr);

};

/// Derived class template implementing the the robust class
template <typename real>
class 
Robust : public IterativeMethod<real> 
{

public:

  typedef typename IterativeMethod<real>::checkT checkT;
  typedef typename IterativeMethod<real>::fctptr fctptr;

  typedef Bisection<real> coarseT;
  typedef Newton<real> fineT;
  
  Robust(real, int, checkT = IterativeMethod<real>::INCREMENT, real = 5e4);

  inline int nitcoarse() { return M_coarse.nit(); }
  inline int nitfine() { return M_fine.nit(); }

  real apply (real, real, fctptr, fctptr);

  template <typename T>
  friend std::ostream& operator << (std::ostream&, Robust<T>&);

private:
  
  real M_cfratio;
  coarseT M_coarse;
  fineT M_fine;
};


///////////////////////////////////
// Class implementations

/// Ctor for the base class
template<typename real>
IterativeMethod<real>::IterativeMethod (real tol, int maxit, checkT check)
  : M_tol(tol), M_maxit(maxit), M_check(check) {};

/// Method for convergence check
template<typename real>
bool IterativeMethod<real>::converged(real increment, real residual) 
{
  return (M_check == INCREMENT && increment < M_tol) ||
    (M_check == RESIDUAL && residual < M_tol);
};

/// Ctor for the bisection class
template<typename real>
Bisection<real>::Bisection (real tol, int maxit, checkT check)
  : IterativeMethod<real> (tol, maxit, check) {};

/// Main method for the bisection class
template<typename real>
real Bisection<real>::apply (real a, real b, fctptr f)
{

  real u = f(a);
  real l = b - a;
  real r;        
  real c = a + l;
  
  this->M_nit = 0;
  r = f(c);

  assert(u*f(b)<0.0);

  while ( !(this->converged(fabs(l), fabs(r)))
          && (this->M_nit <= this->M_maxit) ) {

    (u*r < 0.) ? (b = c) : (a = c, u = r);
    l *= 0.5;
    c = a + l;
    r = f(c);
    ++ this->M_nit;
  }
  return c;
};

/// Ctor for the Newton class
template <typename real>
Newton<real>::Newton (real tol, int maxit) 
  : IterativeMethod<real>(tol, maxit, IterativeMethod<real>::INCREMENT) {};

/// Main method for the Newton class
template <typename real>
real Newton<real>::apply(real xp, fctptr f, fctptr df)
{
  real v = f(xp);
  real xv;
  real derv;
  
  this->M_nit = 0;
  
  do 
    {
      xv = xp;
      derv = df(xv);
      if(!derv) 
        {
          std::cerr << "ERROR: Division by 0 occurred "
            "in Newton::apply()" << std::endl;
          exit (real (1));
        }
      xp = xv - v / derv;
      v = f(xp);
      ++(this->M_nit);
    } 
  while (! (this->converged(fabs(xp - xv), fabs(v)))
         && (this->M_nit <= this->M_maxit));

  return xp;
};

/// Ctor for the Robust class
template <typename real>
Robust<real>::Robust(real tol, int maxit, checkT check, real cfratio)
  :
  IterativeMethod<real>(tol, maxit, check),
  M_cfratio(cfratio),
  M_coarse(M_cfratio * tol, maxit, check),
  M_fine(tol, maxit) {};

/// Main method for the Robust class
template <typename real>
real Robust<real>::apply(real a, real b, fctptr f, fctptr df) 
{
  return M_fine.apply (M_coarse.apply (a, b, f), f, df);
};

template <typename real>
std::ostream& operator<<(std::ostream& ostr, Robust<real>& r) 
{
  ostr << "* Robust Method *" << std::endl
       << "Tolerance           :" << '\t' << r.M_tol
       << std::endl
       << "Max # of iterations :" << '\t' << r.M_maxit
       << std::endl
       << "# of iterations (C) :" << '\t' << r.nitcoarse()
       << std::endl
       << "# of iterations (F) :" << '\t' << r.nitfine()
       << std::endl
       << "C-to-F tol ratio    :" << '\t' << r.M_cfratio
       << std::flush;
  return ostr;
};

#endif
