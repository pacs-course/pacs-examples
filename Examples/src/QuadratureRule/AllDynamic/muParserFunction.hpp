/*
 * muParserFunction.hpp
 *
 *  Created on: Feb 11, 2021
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_QUADRATURERULE_ALLDYNAMIC_MUPARSERFUNCTION_HPP_
#define EXAMPLES_SRC_QUADRATURERULE_ALLDYNAMIC_MUPARSERFUNCTION_HPP_
#include "muParser.h"
#include <string>
 namespace apsc::MuParserInterface
{
  class muParserFunction
  {
  public:
    muParserFunction()=default;
    muParserFunction(std::string filename);
    ~muParserFunction();
    muParserFunction(muParserFunction const &);
    muParserFunction & operator=(muParserFunction const &);
    void set_expression(const std::string & e);
    //! A generic operator
    /*! Takes as II argument anything that can be addressed
      as coord[0] and coord[1];
    */
    inline double operator()(double const & x);
  private:
    mu::Parser M_parser;
    double M_x;
    std::string M_expr;
    static inline const std::string filename{"parsedFunction.txt"};
  };


  inline double
  muParserFunction::operator()(double const &x)
  {
    this->M_x = x;
    return this->M_parser.Eval();
  }
}// end namespace


#endif /* EXAMPLES_SRC_QUADRATURERULE_ALLDYNAMIC_MUPARSERFUNCTION_HPP_ */
