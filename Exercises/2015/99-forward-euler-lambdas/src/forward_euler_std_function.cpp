#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <functional>

template<typename T>
void setMaxPrecison (std::ostream & out)
{
  auto maxprec = std::numeric_limits<T>::digits10;
  out.precision (maxprec + 1);
}

template<typename real>
class 
forward_euler
{
public:
  
  forward_euler (real _start, real _stop,
                 real _dt0, real _tol) :
    start (_start), stop (_stop), 
    dt0 (_dt0), tol (_tol)
  {};

  void
  apply (std::function<real(real, real)> fun,
         real x0, 
         std::vector<real>& result,
         std::vector<real>& time) 
  {
    real t = start;
    real dt = dt0;
    real x = x0;
    real xold = x;
    real x2 = x;
    real err = 10 * tol;
    real told = t;
    real xprime = 0;

    result.clear ();
    time.clear ();
    
    result.push_back (x);
    time.push_back (t);

    while (t < stop)      
      {

        xold = x;
        told = t;
        xprime = fun (x, t);
        
        t += dt;
        x += xprime * dt;
        xprime = fun (x, t);

        real x2 = .5 * (x + xold + xprime * dt);
        real err = fabs (x2 - x);
        if (err <= tol)
          {
            result.push_back (x);
            time.push_back (t); 
          }
        else
          {
            std::cout << "%% reject" << std::endl;
            x = xold;
            t = told;
          }

        std::cout << "% t = " << t 
                  << ";\t x = " << x 
                  << std::endl;

        dt *= .5 * sqrt (tol / err);
        dt = t + dt > stop ? stop - t : dt;
      }      
  }
  
private:

  real start, stop, dt0, tol;
  
};

template<typename real>
real fun (real x, real t, real k)
{
  return (-sin (k * x));
};

int main (void)
{
  std::vector<double> x;
  std::vector<double> t;

  forward_euler<double> f (0.0, 100.0, .1, 1e-3);
  f.apply ([] (double xx, double tt) 
           {return fun<double> (xx, tt, 1.0 / 2.0);}, 
           10, x, t);

  setMaxPrecison<double> (std::cout);
  std::cout << "r = [" << std::endl;
  for (auto ii = x.begin (), jj = t.begin (); 
       ii != x.end () || jj != t.end (); 
       ++ii, ++jj)
    {
      std::cout << *jj << ",\t " << *ii << std::endl;
    }
  
  std::cout << "];" << std::endl;

  return 0;
}
