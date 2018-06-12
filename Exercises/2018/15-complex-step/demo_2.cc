#include <iomanip>
#include <iostream>
#include <complex-step.h>

// This shows how diff works: when
// invoking "Quad" directly the double
// version of "quad" is invoked once
// when invoking "diff (Quad, x)" the
// complex version is invoked twice.

std::complex<double>
quad (std::complex<double> x)
{
  std::cout << " complex ";
  return x * x;
}

double
quad (double x)
{
  std::cout << " double ";
  return x * x;
}


int
main ()
{

  auto Quad = [] (auto x) { return quad (x); };
  auto d_Quad_d_x = diff (Quad);
  
  for (double x = 0.0; x < 0.002; x += 0.001)
    std::cout << std::setprecision (15) << std::scientific << x << " "
              << Quad (x) << " " << diff (Quad, x) << " " << d_Quad_d_x (x) 
              << std::endl;
  
  return 0;
}
