/*
 * muParserFunction.cpp
 *
 *  Created on: Feb 11, 2021
 *      Author: forma
 */
#include "muParserFunction.hpp"
#include <iostream>
#include <fstream>
namespace apsc::MuParserInterface
{
  muParserFunction::muParserFunction(std::string filename) {
    this->M_parser.DefineVar("x",&M_x);
    std::fstream theFunction{filename, std::ios::in};
    std::string expression;
    if(theFunction.fail() || theFunction.eof())
      {
        std::cerr<<"No valid muparser function\n";
        expression="0.*x";
      }
    else
      {
        std::getline(theFunction,expression);
        std::cout<<"Read parsed function= "<<expression<<std::endl;
      }
    theFunction.close();
    this->set_expression(expression);
  }

  muParserFunction::~muParserFunction() {this->M_parser.ClearVar();}

  muParserFunction::muParserFunction(const muParserFunction& mf):M_parser(),M_x{mf.M_x},M_expr(mf.M_expr)
      {
    this->M_parser.SetExpr(M_expr);
    this->M_parser.DefineVar("x",&M_x);
      }

  muParserFunction &
  muParserFunction::operator=(const muParserFunction & mf)
  {
    if (this != &mf)
      {
        this->M_parser.ClearVar();
        this->M_expr=mf.M_expr;
        this->M_x=mf.M_x;
        this->M_parser.DefineVar("x",&M_x);
        this->M_parser.SetExpr(M_expr);
      }
    return *this;

  }

  void
  muParserFunction::set_expression(const std::string &e)
  {
    this->M_expr=e;
    this->M_parser.SetExpr(e);
  }
}



