#ifndef HH_MUPARSERINTERFACE_HH
#define HH_MUPARSERINTERFACE_HH
#include <string>
#include "muParser.h"
namespace MuParserInterface
{
  class muParserInterface
  {
  public:
    muParserInterface();
    muParserInterface(const std::string & s);
    ~muParserInterface();
    muParserInterface(muParserInterface const &);
    muParserInterface & operator=(muParserInterface const &);
    
    void set_expression(const std::string & e);
    double operator()(double const t, double const * coord);
  private:
    mu::Parser M_parser;
    double M_t,M_x,M_y;
    std::string M_expr;
  };
  
  //! prints message on the standard error
  void printMuException(mu::Parser::exception_type &e);
}// end namespace
#endif
