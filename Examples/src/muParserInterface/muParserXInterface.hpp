#ifndef HH_MUPARSERXINTERFACE_HH
#define HH_MUPARSERXINTERFACE_HH
#include "mpParser.h"
#include <array>
#include <iostream>
#include <string>
namespace MuParserInterface
{
/**
 * @brief Wrapper around muParserX for expressions using vector variable `x`.
 *
 * The represented function is R^N -> R. Input components are exposed inside
 * the expression as `x[0]`, `x[1]`, ..., `x[N-1]`.
 *
 * Example expression: `sin(x[0]) + x[1] * x[2]`.
 *
 * @tparam N Number of input arguments (fixed at compile time).
 * @tparam ArgumentType Input container type supporting `operator[]`.
 */
template <int N, class ArgumentType = std::array<double, N> >
class muParserXInterface
{
public:
  /**
   * @brief Build parser and bind variable `x`.
   *
   * Packages:
   * - `mup::pckALL_NON_COMPLEX`: enable standard non-complex features.
   * - `mup::pckMATRIX`: enable array/matrix constructs, needed for `x[i]`.
   */
  muParserXInterface()
    : My_e(), M_parser(mup::pckALL_NON_COMPLEX | mup::pckMATRIX),
      M_value{N, 0.0}
  {
    M_parser.DefineVar("x", mup::Variable(&M_value));
  }
  /// Build parser and set an initial expression.
  muParserXInterface(const std::string expression) : muParserXInterface()
  {
    My_e = expression;
    M_parser.SetExpr(My_e.c_str());
  }

  /*!
   * @brief Copy constructor with explicit parser reinitialization.
   *
   * muParserX stores variable addresses internally. A default copy would keep
   * stale pointers to the source object state. This constructor rebuilds parser
   * bindings and restores the expression from `My_e`.
   *
   * @param mpi Source object.
   */
  muParserXInterface(muParserXInterface const &mpi)
    : My_e(mpi.My_e), M_parser(mup::pckALL_NON_COMPLEX | mup::pckMATRIX),
      M_value{N, 0.0}
  {
    M_parser.DefineVar("x", mup::Variable(&M_value));
    M_parser.SetExpr(My_e.c_str());
  }
  /*!
   * @brief Copy assignment with parser rebinding.
   *
   * Same rationale as the copy constructor: clear old bindings, copy values and
   * expression, then re-register variable `x`.
   *
   * @param mpi Source object.
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
   * Input variables must be written as `x[index]`, e.g.
   * `sin(x[0]) + x[1] * x[2]`.
   *
   * @param e Expression text.
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
    // Copy user inputs into the parser-bound value container.
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
        std::cerr << "muParserX error with code: " << error.GetCode()
                  << std::endl;
        std::cerr << "While processing expression: " << error.GetExpr()
                  << std::endl;
        std::cerr << "Error Message: " << error.GetMsg() << std::endl;
        throw error;
      }
    return ans.GetFloat();
  }

private:
  /// Local expression copy used to safely rebuild parser state after copy.
  std::string My_e;
  /// muParserX engine.
  mup::ParserX M_parser;
  /// Value container bound to parser variable `x`.
  mutable mup::Value M_value;
};
} // namespace MuParserInterface
#endif
