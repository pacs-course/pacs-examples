#ifndef ITERATIVEMETHOD_HPP
#define ITERATIVEMETHOD_HPP 1

#include <cassert>
#include <cmath>

///////////////////////////////////
// Class declarations

/// Base class template for iterative methods
template <typename real, class fun>
class IterativeMethod 
{
public:

  enum checkT {
    INCREMENT,
    RESIDUAL  
  };

  IterativeMethod (real tol, int maxit, checkT check);
  
  inline real& tol() { return M_tol; }
  inline int& maxit() { return M_maxit; }
  inline int nit() const { return M_nit; }
  
protected:
  
  bool converged (real, real);

  int    M_nit;
  real   M_tol;
  int    M_maxit;
  checkT M_check;
  fun    f;

};

/// Derived class template implementing the bisection method
template <typename real, class fun>
class Bisection : public IterativeMethod<real, fun> {

public:

  typedef typename IterativeMethod<real, fun>::checkT checkT;

  Bisection (real tol, int maxit, checkT check);
  real apply (real, real);

};

/// Derived class template implementing the Newton method
template <typename real, class fun>
class Newton : public IterativeMethod<real, fun> {
public:

  typedef typename IterativeMethod<real, fun>::checkT checkT;

  Newton (real, int);
  real apply (real);

};

/// Derived class template implementing the the robust class
template <typename real, class fun>
class 
Robust : public IterativeMethod<real, fun> 
{

public:

  typedef typename IterativeMethod<real, fun>::checkT checkT;

  typedef Bisection<real, fun> coarseT;
  typedef Newton<real, fun> fineT;
  
  Robust (real, int, 
          checkT = IterativeMethod<real, fun>::INCREMENT, 
          real = 5e4);

  inline int nitcoarse() { return M_coarse.nit(); }
  inline int nitfine() { return M_fine.nit(); }

  real apply (real, real);

  template <typename T, class W>
  friend std::ostream& 
  operator << 
  (std::ostream&, Robust<T, W>&);

private:
  
  real M_cfratio;
  coarseT M_coarse;
  fineT M_fine;
};


///////////////////////////////////
// Class implementations

/// Ctor for the base class
template<typename real, class fun>
IterativeMethod<real, fun>::IterativeMethod 
(real tol, int maxit, checkT check)
  : M_tol(tol), M_maxit(maxit), M_check(check) {};

/// Method for convergence check
template<typename real, class fun>
bool IterativeMethod<real, fun>::converged
(real increment, real residual) 
{
  return (M_check == INCREMENT && increment < M_tol) ||
    (M_check == RESIDUAL && residual < M_tol);
};

/// Ctor for the bisection class
template<typename real, class fun>
Bisection<real, fun>::Bisection 
(real tol, int maxit, checkT check)
  : IterativeMethod<real, fun> (tol, maxit, check) {};

/// Main method for the bisection class
template<typename real, class fun>
real Bisection<real, fun>::apply (real a, real b)
{

  real u = this->f(a);
  real l = b - a;
  real r;        
  real c = a + l;
  
  this->M_nit = 0;
  r = this->f(c);

  assert(u * this->f (b) < 0.0);

  while ( !(this->converged(fabs(l), fabs(r)))
          && (this->M_nit <= this->M_maxit) ) {

    (u*r < 0.) ? (b = c) : (a = c, u = r);
    l *= 0.5;
    c = a + l;
    r = this->f(c);
    ++ this->M_nit;
  }
  return c;
};

/// Ctor for the Newton class
template <typename real, class fun>
Newton<real, fun>::Newton (real tol, int maxit) 
  : IterativeMethod<real, fun>
    (tol, maxit, IterativeMethod<real, fun>::INCREMENT) {};

/// Main method for the Newton class
template <typename real, class fun>
real Newton<real, fun>::apply (real xp)
{
  real v = this->f(xp);
  real xv;
  real derv;
  
  this->M_nit = 0;
  
  do 
    {
      xv = xp;
      derv = this->f.df(xv);
      if(!derv) 
        {
          std::cerr << "ERROR: Division by 0 occurred "
            "in Newton::apply()" << std::endl;
          exit (real (1));
        }
      xp = xv - v / derv;
      v = this->f(xp);
      ++(this->M_nit);
    } 
  while (! (this->converged(fabs(xp - xv), fabs(v)))
         && (this->M_nit <= this->M_maxit));

  return xp;
};

/// Ctor for the Robust class
template <typename real, class fun>
Robust<real, fun>::Robust
(real tol, int maxit, checkT check, real cfratio)
  :
  IterativeMethod<real, fun>(tol, maxit, check),
  M_cfratio(cfratio),
  M_coarse(M_cfratio * tol, maxit, check),
  M_fine(tol, maxit) {};

/// Main method for the Robust class
template <typename real, class fun>
real Robust<real, fun>::apply(real a, real b) 
{
  return M_fine.apply (M_coarse.apply (a, b));
};

template <typename real, class fun>
std::ostream& 
operator<<
(std::ostream& ostr, Robust<real, fun>& r) 
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
