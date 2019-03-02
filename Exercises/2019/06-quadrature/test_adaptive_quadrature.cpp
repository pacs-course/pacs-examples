#include "GetPot"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <functional>
#include <string>
#include "muparser_fun.h"
#include "adaptive_quadrature.h"

int main (int argc, char ** argv)
{

  GetPot g (argc, argv);
  std::string filename = g ("filename", "data.txt");

  GetPot g2 (filename.c_str ());
  std::string fun_str   = g2 ("fun", "(sin(x^2))^2");
  std::string int_str   = g2 ("integrator", "trapz");
  double start          = g2 ("start", 0.0);
  double finish         = g2 ("finish",  4.0*atan (1.0));
  double tol            = g2 ("tol",  1.e-12);
  int maxdepth          = g2 ("maxdepth",  40);

  muparser_fun fun (fun_str);

  adaptive_quadrature aq (fun, start, finish, tol, maxdepth, int_str);
  std::cout << "integration result = " << aq.integrate () << std::endl;
  
  return 0;
}
