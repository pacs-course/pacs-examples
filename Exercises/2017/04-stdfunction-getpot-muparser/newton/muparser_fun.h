
#include <muParser.h>
#include <string>

class muparser_fun
{
private :

  std::string expr;
  double var;
  mu::Parser p;
  
public :

  muparser_fun (const std::string & s) : expr (s)                          
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
