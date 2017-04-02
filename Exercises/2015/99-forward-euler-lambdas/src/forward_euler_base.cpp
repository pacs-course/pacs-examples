#include <iostream>
#include <vector>
#include <cmath>

typedef double real;

int main (void)
{

  real start = 0;
  real stop = 10;
  real dt0 = .1;
  real tol = 1e-3;
  real x0 = 10;

  std::vector<double> result;
  std::vector<double> time;

  real t = start;
  real dt = dt0;
  real x = x0;
  real k = .5;
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
      xprime = -k * x;
        
        t += dt;
        x += xprime * dt;
        xprime = - k * x;

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

        std::cout << "% t = " 
                  << t << ";\t x = " 
                  << x << std::endl;

        dt *= .5 * sqrt (tol / err);
        dt = t + dt > stop ? stop - t : dt;
    }      

  std::cout << "r = [" << std::endl;
  for (auto ii = result.begin (),  jj = time.begin (); 
       ii != result.end () || jj != time.end (); 
       ++ii, ++jj)
    {
      std::cout << *jj << ",\t " << *ii << std::endl;
    }
  
  std::cout << "];" << std::endl;

  return 0;
}
