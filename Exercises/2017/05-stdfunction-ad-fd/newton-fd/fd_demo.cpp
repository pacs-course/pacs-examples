#include "newton.h"
#include "muparser_fun.h"
#include <GetPot>
#include <iomanip>

constexpr double piby4  = 7.85398163397448e-01;

int
main (int argc, char ** argv)
{

  GetPot g (argc, argv);
  
  double start = g ("start", -3.0);
  double stop  = g ("stop",  -8.0);
  double step  = g ("step",  -1.0e-2);
  
  std::string fun_str = g ("fun", "sin(x)");
  std::string der_str = g ("fun", "cos(x)");

  muparser_fun fun (fun_str);
  muparser_fun der (der_str);
  

  for (double x = start; x > stop; x += step)
    {

      newton_solver n (fun, 0, .0, .0, (std::pow(10.0,x)));

      std::cout << std::setprecision (17) << (std::pow(10.0,x)) << ", "
                << n.prime (piby4) << ", "
                << der (piby4) << std::endl;
    }
      
  
  return 0;
}
