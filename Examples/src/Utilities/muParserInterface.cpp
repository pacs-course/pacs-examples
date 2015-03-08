#include <iostream>
#include "muParserInterface.hpp"
namespace MuParserInterface
{
  muParserInterface::muParserInterface()
  {
    this->M_parser.DefineVar("t",&M_t);
    this->M_parser.DefineVar("x",&M_x);
    this->M_parser.DefineVar("y",&M_y);
  }

  
  muParserInterface::muParserInterface(const std::string & e):
    muParserInterface()
  { 
    M_expr=e;
    this->M_parser.SetExpr(e);
  }

  muParserInterface::~muParserInterface()
  {
    this->M_parser.ClearVar();
  }

  muParserInterface::muParserInterface(muParserInterface const & mpi):
    M_parser(),M_t(mpi.M_t),M_x(mpi.M_x),M_y(mpi.M_y),M_expr(mpi.M_expr)
  {
    this->M_parser.SetExpr(M_expr);
    this->M_parser.DefineVar("t",&M_t);
    this->M_parser.DefineVar("x",&M_x);
    this->M_parser.DefineVar("y",&M_y);    
  }

  muParserInterface & muParserInterface::operator=(muParserInterface const & mpi)
  {
    if (this != &mpi)
      {
	this->M_parser.ClearVar();
	this->M_expr=mpi.M_expr;
	this->M_t=mpi.M_t;
	this->M_x=mpi.M_x;
	this->M_y=mpi.M_y;
	this->M_parser.SetExpr(M_expr);
	this->M_parser.DefineVar("t",&M_t);
	this->M_parser.DefineVar("x",&M_x);
	this->M_parser.DefineVar("y",&M_y);
      }
    return *this;
  }

  void  muParserInterface::set_expression(const std::string & s)
  {
    M_expr=s;
    this->M_parser.SetExpr(s);
  }

  double
  muParserInterface::operator()(double const t, double const * coord)
  {
    this->M_t=t;
    this->M_x = coord[0];
    this->M_y = coord[1];
    
    /* OLNY FOR DEBUGGING
    // Get the map with the variables
    mu::varmap_type variables = M_parser.GetVar();
    std::cout << "Number: " << (int)variables.size() << "\n";
    
    // Get the number of variables 
    mu::varmap_type::const_iterator item = variables.begin();
    
    // Query the variables
    for (; item!=variables.end(); ++item)
      {
	std::cout << "Name: " << item->first << " Value" << *item->second << "\n";
      }
    */
    
    return this->M_parser.Eval();
  }

  void printMuException(mu::Parser::exception_type &e)
  {
    using std::cout;
    cout << "Content of muParser exception" << "\n";
    cout << "Message:  " << e.GetMsg() << "\n";
    cout << "Formula:  " << e.GetExpr() << "\n";
    cout << "Token:    " << e.GetToken() << "\n";
    cout << "Position: " << e.GetPos() << "\n";
    cout << "Errc:     " << e.GetCode() << "\n";
  }
}// end namespace
