#include <iostream>
#include <limits>
#include <vector>
#include <cmath>
#include <octave/oct.h>
#include <octave/octave.h>
#include <octave/parse.h>
#include <octave/toplev.h>


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

  template<class T>
  void apply (T fun, real x0, 
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
real fun (real x, real t, std::string s)
{
  octave_value_list out, in;
  in(1) = octave_value (t);
  in(0) = octave_value (x);
  out = feval (s, in);
  return (out(0).double_value ());
};

int main (int argc, char **argv)
{
  string_vector ARGV (2);
  ARGV(0) = "embedded";
  ARGV(1) = "-q";

  octave_main (ARGV.numel (), ARGV.c_str_vec (), 1);
  
  std::vector<double> x;
  std::vector<double> t;

  forward_euler<double> f (0.0, 100.0, .1, 1e-3);
  f.apply ([argv] (double xx, double tt) 
           {return fun<double> (xx, tt, argv[1]);}, 
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

  clean_up_and_exit (0);
}
