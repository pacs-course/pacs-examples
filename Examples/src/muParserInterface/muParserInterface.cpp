#include "muParserInterface.hpp"
#include <array>
#include <iostream>
namespace MuParserInterface
{
muParserInterface::muParserInterface()
{
  this->M_parser.DefineVar("t", &M_t);
  this->M_parser.DefineVar("x", &M_x);
  this->M_parser.DefineVar("y", &M_y);
}

muParserInterface::muParserInterface(const std::string &e) : muParserInterface()
{
  M_expr = e;
  this->M_parser.SetExpr(e);
}

muParserInterface::~muParserInterface() { this->M_parser.ClearVar(); }

double
muParserInterface::operator()(double const t, double const x, double const y) const
{
  return this->operator()(t, std::array<double, 2>{{x, y}});
}

muParserInterface::muParserInterface(muParserInterface const &mpi)
  : M_parser(), M_t(mpi.M_t), M_x(mpi.M_x), M_y(mpi.M_y), M_expr(mpi.M_expr)
{
  this->M_parser.SetExpr(M_expr);
  this->M_parser.DefineVar("t", &M_t);
  this->M_parser.DefineVar("x", &M_x);
  this->M_parser.DefineVar("y", &M_y);
}

muParserInterface &
muParserInterface::operator=(muParserInterface const &mpi)
{
  if(this != &mpi)
    {
      this->M_parser.ClearVar();
      this->M_expr = mpi.M_expr;
      this->M_t = mpi.M_t;
      this->M_x = mpi.M_x;
      this->M_y = mpi.M_y;
      this->M_parser.SetExpr(M_expr);
      this->M_parser.DefineVar("t", &M_t);
      this->M_parser.DefineVar("x", &M_x);
      this->M_parser.DefineVar("y", &M_y);
    }
  return *this;
}

void
muParserInterface::set_expression(const std::string &s)
{
  M_expr = s;
  this->M_parser.SetExpr(s);
}

void
printMuException(mu::Parser::exception_type &e)
{
  using std::cout;
  cout << "Content of muParser exception"
       << "\n";
  cout << "Message:  " << e.GetMsg() << "\n";
  cout << "Formula:  " << e.GetExpr() << "\n";
  cout << "Token:    " << e.GetToken() << "\n";
  cout << "Position: " << e.GetPos() << "\n";
  cout << "Errc:     " << e.GetCode() << "\n";
}
} // namespace MuParserInterface
