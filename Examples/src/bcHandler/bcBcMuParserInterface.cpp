#include "bcBcMuParserInterface.hpp"

#include <algorithm>
#include <iostream>
namespace apsc::FEM
{
bcMuParserInterface::bcMuParserInterface()
{
  M_parser.DefineVar("x", mup::Variable(&mup_x));
  M_parser.DefineVar("t", mup::Variable(&mup_t));
}
bcMuParserInterface::bcMuParserInterface(const std::string &e)
  : bcMuParserInterface()
{
  My_e = e;
  M_parser.SetExpr(e.c_str());
}

bcMuParserInterface::bcMuParserInterface(bcMuParserInterface const &mpi)
  : My_e{mpi.My_e}, M_parser{static_cast<unsigned int>(mup::pckALL_NON_COMPLEX |
                                                       mup::pckMATRIX)},
    mup_x{3, 0.}, mup_t{0.}
{
  M_parser.DefineVar("x", mup::Variable(&mup_x));
  M_parser.DefineVar("t", mup::Variable(&mup_t));
  M_parser.SetExpr(mpi.My_e.c_str());
}
bcMuParserInterface &
bcMuParserInterface::operator=(bcMuParserInterface const &mpi)
{
  if(this != &mpi)
    {
      this->My_e = mpi.My_e;
      this->M_parser.ClearVar();
      this->mup_t = mpi.mup_t;
      this->mup_x = mpi.mup_x;
      M_parser.DefineVar("x", mup::Variable(&mup_x));
      M_parser.DefineVar("t", mup::Variable(&mup_t));
      this->M_parser.SetExpr(mpi.My_e.c_str());
    }
  return *this;
}

void
bcMuParserInterface::set_expression(const std::string &e)
{
  M_parser.SetExpr(e.c_str());
};

double
bcMuParserInterface::operator()(double const &t, apsc::FEM::Coord x)
{
  // max 3 coordinates
  auto n = std::min(3ul, x.size());
  this->mup_t = t;
  for(unsigned int i = 0; i < n; ++i)
    this->mup_x.At(i) = x[i];
  mup::Value res;
  try
    {
      res = this->M_parser.Eval();
    }
  catch(mup::ParserError &error)
    {
      std::cerr << "Muparsex error with code:" << error.GetCode() << std::endl;
      ;
      std::cerr << "While processing expression: " << error.GetExpr()
                << std::endl;
      std::cerr << "Error Message: " << error.GetMsg() << std::endl;
      throw error;
    }
  return res;
}
} // namespace apsc::FEM
