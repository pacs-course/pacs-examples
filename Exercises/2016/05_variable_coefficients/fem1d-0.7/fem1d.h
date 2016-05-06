#ifndef HAVE_FEM1D_H
#define HAVE_FEM1D_H 

#include <array>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/SparseLU>
#include <muParser.h>
#include <string>

std::string help_text = std::string () +
"fem1d command line options\n" +
  "-h, --help              print this text and exit\n" +
  "-a <value>              first end of interval\n" +
  "-b <value>              second end of interval\n" +
  "-d, --diffusion <value> expression to compute diffusion coefficient\n" +
  "-f, --forcing <value>   expression to compute forcing term coefficient\n" +
  "-n, --nnodes <value>    number of triangulation nodes\n" +
  "-m, --maxit <value>     number of iterations\n" +
  "-t, --tol <value>       tolerance\n";
  
class coeff
{
private :

  std::string expr;
  double var;
  mu::Parser p;
  
public :

  coeff (const std::string & s) : expr (s)                          
  {
    try
      {
        p.DefineVar ("x", &var);
        p.SetExpr (expr.c_str ());
      }
    catch (mu::Parser::exception_type &e)
      {
        std::cerr << e.GetMsg () << std::endl;
      }
  };

  double operator() (double x)
  {
    double y;
    var = x;
    try
      {
        y = p.Eval ();
      }
    catch (mu::Parser::exception_type &e)
      {
        std::cerr << e.GetMsg () << std::endl;
      }
    return (y);
  };
};
#endif
