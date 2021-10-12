#ifndef HH_MUPARSERINTERFACE_HH
#define HH_MUPARSERINTERFACE_HH
#include "bcType_traits.hpp"
#include "mpParser.h"
#include <array>
#include <string>

namespace apsc::FEM
{
class bcMuParserInterface
{
public:
  bcMuParserInterface();
  // I need compy constructor
  bcMuParserInterface(bcMuParserInterface const &);
  bcMuParserInterface &operator=(bcMuParserInterface const &);
  bcMuParserInterface(const std::string &e);

  void set_expression(const std::string &e);

  double operator()(double const &t, apsc::FEM::Coord x);

private:
  std::string My_e;
  // static_cast is not needed. Is here only becouse otherwise the stupid
  // eclipse complains!
  mup::ParserX M_parser{
    static_cast<unsigned int>(mup::pckALL_NON_COMPLEX | mup::pckMATRIX)};
  mup::Value mup_x{3, 0.};
  mup::Value mup_t{0.};
};

} // namespace apsc::FEM
#endif
