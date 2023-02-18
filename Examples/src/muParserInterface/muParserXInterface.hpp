#ifndef HH_MUPARSERXINTERFACE_HH
#define HH_MUPARSERXINTERFACE_HH
#include "mpParser.h"
#include <array>
#include <iostream>
#include <string>
namespace MuParserInterface
{
/*! An interface to MuParserX to define a function

  It define a functor representing a function \f$ R^N \f$ to \f$ R\f$
  The input variables are defined as x[0] x[1] etc and one can use the muparserX
  syntax to create the expression.

  I assume that at compile time we know the size of the argument of the function
  and I keep the return value as template parameter. By default both input and
  output are arrays.
  The input variables are indicated by x[]. An example of a valid expression:
  sin(x[0])+x[1]*x[2]

  @tparam N The number of arguments of the function we want to represent
  @tparam ArgumentType any type that support the addressing ([]) operator
*/
template <int N, class ArgumentType = std::array<double, N> >
class muParserXInterface
{
public:
  //! Default constructor
  //!
  //! mup::pckALL_NON_COMPLEX|mup::pckMATRIX means that I do not want the module
  //! for complex numbers but I want to treat arrays and matrices in muparserX
  //! expressions
  muParserXInterface()
    : My_e(),
      M_parser(mup::pckALL_NON_COMPLEX | mup::pckMATRIX), M_value{N, 0.0}
  {
    M_parser.DefineVar("x", mup::Variable(&M_value));
  }
  //! Constructor that takes a string containing muParserX expression
  muParserXInterface(const std::string expression) : muParserXInterface()
  {
    My_e = expression;
    M_parser.SetExpr(My_e.c_str());
  }

  /*!
   * The copy constructor
   *
   * MuparserX has a particular design, which obliges to define a special copy
   * constructor The reson is that a muparser engine stores the address of the
   * variables. So a normal copy would do a shallow copy, which is NOT what you
   * want. Moreover, because of a poor design, you may loose the expression.
   * That's why I keep a copy in the class as a string and a redefine in in the
   * muparser engine.
   *
   * @param mpi the muParserXInterface to be copied
   */
  muParserXInterface(muParserXInterface const &mpi)
    : My_e(mpi.My_e),
      M_parser(mup::pckALL_NON_COMPLEX | mup::pckMATRIX), M_value{N, 0.0}
  {
    M_parser.DefineVar("x", mup::Variable(&M_value));
    M_parser.SetExpr(My_e.c_str());
  }
  /*!
   * The copy assignment operator
   *
   * MuparserX has a particular design, which obliges to define a special copy
   * assignement
   * @param mpi the muParserXInterface to be copied
   * The copy constructor
   */
  muParserXInterface
  operator=(muParserXInterface const &mpi)
  {
    if(this != &mpi)
      {
        this->My_e = mpi.My_e;
        this->M_parser.ClearVar(); // clear the variables!
        this->M_value = mpi.M_value;
        M_parser.DefineVar("x", mup::Variable(&M_value));
        M_parser.SetExpr(My_e.c_str());
      }
    return *this;
  }

  //! Sets the muparserX expression.
  /*!
   * Beware, the input variables are indicated by x[].
   * example of a valid expression: sin(x[0])+x[1]*x[2]
   * @par e The expression
   */
  void
  set_expression(const std::string &e)
  {
    My_e = e;
    M_parser.SetExpr(e.c_str());
  }

  auto
  operator()(ArgumentType const &x) const
  {
    for(int i = 0; i < N; ++i)
      {
        M_value.At(i) = x[i];
      }
    mup::Value ans;
    try
      {
        ans = M_parser.Eval();
      }
    catch(mup::ParserError &error)
      {
        std::cerr << "Muparsex error with code:" << error.GetCode()
                  << std::endl;
        ;
        std::cerr << "While processing expression: " << error.GetExpr()
                  << std::endl;
        std::cerr << "Error Message: " << error.GetMsg() << std::endl;
        throw error;
      }
    return ans.GetFloat();
  }

private:
  // a copy of the muparserX expression, used for the copy operations
  std::string My_e;
  // The muparseX engine
  mup::ParserX M_parser;
  // The muparserX value used to set the variables in the engine
  mutable mup::Value M_value;
};
} // namespace MuParserInterface
#endif
