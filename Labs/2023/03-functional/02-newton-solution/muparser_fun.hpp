#include <muParser.h>

#include <memory>
#include <string>

class MuparserFun
{
public:
  MuparserFun(const MuparserFun &m)
    : m_parser(m.m_parser)
  {
    m_parser.DefineVar("x", &m_var);
  };

  MuparserFun(const std::string &s)
  {
    try
      {
        m_parser.DefineVar("x", &m_var);
        m_parser.SetExpr(s);
      }
    catch (mu::Parser::exception_type &e)
      {
        std::cerr << e.GetMsg() << std::endl;
      }
  };

  double
  operator()(const double &x)
  {
    double y = 0;

    m_var = x;

    try
      {
        y = m_parser.Eval();
      }
    catch (mu::Parser::exception_type &e)
      {
        std::cerr << e.GetMsg() << std::endl;
      }
    return y;
  };

private:
  double     m_var;
  mu::Parser m_parser;
};
