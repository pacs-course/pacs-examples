#ifndef HH_MUPARSERINTERFACE_HH
#define HH_MUPARSERINTERFACE_HH
#include "muParser.h"
#include <string>
namespace MuParserInterface
{
class muParserInterface
{
public:
  muParserInterface();
  muParserInterface(const std::string &s);
  ~muParserInterface();
  muParserInterface(muParserInterface const &);
  muParserInterface &operator=(muParserInterface const &);

  void set_expression(const std::string &e);
  //! A generic operator
  /*! Takes as II argument anything that can be addressed
    as coord[0] and coord[1];
  */
  template <typename COORD>
  double operator()(double const t, COORD const &coord) const;
  //! Version that takes x and y
  double operator()(double const t, double const x, double const y) const;

private:
  mu::Parser  M_parser;
  mutable double M_t;
  mutable double M_x;
  mutable double M_y;
  std::string M_expr;
};

template <typename COORD>
double
muParserInterface::operator()(double const t, COORD const &coord)const
{
  this->M_t = t;
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

//! prints message on the standard error
void printMuException(mu::Parser::exception_type &e);
} // namespace MuParserInterface
#endif
