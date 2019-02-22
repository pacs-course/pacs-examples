#ifndef HH_MUPARSERXINTERFACE_HH
#define HH_MUPARSERXINTERFACE_HH
#include <string>
#include "mpParser.h"
#include <array>
#include <string>
#include <iostream>
namespace MuParserInterface
{
  /*! An interface to MuParserX to define a function
 
    It define a functor representing a function \f$ R^N \f$ to \f$ R\f$
    The input variables are defined as x[0] x[1] etc and one can use the 
    muparserX syntax to create the string.
    I assume that at compile time we know the size of the argument of the function 
    and I keep the return value as template parameter. By default both input and 
    output are arrays.
    
    @note I am not catching the exception thrown if the muParserX
    expression is badly formed.

    @tpar N the size of the input
    @tpar ArgumentType any type that support the addressing ([]) operator
  */
  template<int N, class ArgumentType=std::array<double,N> > 
  class muParserXInterface
  {
  public:
    //! Default constructor
    muParserXInterface():M_parser(mup::pckALL_NON_COMPLEX|mup::pckMATRIX),
                         M_x(),M_value(N,0.0)
    {
      M_parser.DefineVar("x",mup::Variable(&M_value));
    }
    //! Constructor that takes a muParserX expression.
    /*!
     * Beware, the input variables are indicated by x[]. 
     * example of a valid expression: sin(x[0])+x[1]*x[2]
     */
    muParserXInterface(const std::string s):muParserXInterface()
    {
      M_parser.SetExpr(s.c_str());
    }

    //! Sets the muparser expression.
    /*!
     * Beware, the input variables are indicated by x[]. 
     * example of a valid expression: sin(x[0])+x[1]*x[2]
     * @par e The expression
     */
    void set_expression(const std::string & e)
    {
      M_parser.SetExpr(e.c_str());
    }
    
    double operator()(ArgumentType const & x){
      
      for (int i = 0; i<N; ++i)
        {
          M_value.At(i)=x[i];
        }
      mup::Value ans;
      try
        {
          ans = M_parser.Eval();
        }
      catch (mup::ParserError & e)
        {
          std::cerr<<"Error "<<e.GetMsg()<<std::endl;
          return 0;
        }
      return ans.GetFloat();
    }

  private:
    mup::ParserX M_parser;
    ArgumentType M_x;
    mup::Value M_value;
  };
}// end namespace
#endif
