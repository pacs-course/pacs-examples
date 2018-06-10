#ifndef HAVE_COMPLEX_STEP_H
#define HAVE_COMPLEX_STEP_H

#include <iostream>
#include <cmath>
#include <complex>
#include <limits>

template<class FUN>
double
diff (const FUN& fun, const double x)
  {
    double y;
    double h = 1.0e-100;
    
    y = ((fun (std::complex<double> (x, h)) -
          fun (std::complex<double> (x, 0))) /
         std::complex<double> (h, 0)).imag ();

    return y;
  }

template<class FUN>
auto
diff (const FUN& fun)
  {
    auto f = [fun] (double x) { return diff (fun, x); }; 
    return f;
  }


#endif
