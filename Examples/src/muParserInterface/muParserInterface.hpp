#ifndef HH_MUPARSERINTERFACE_HH
#define HH_MUPARSERINTERFACE_HH
#include "muParser.h"
#include <string>
namespace MuParserInterface
{
/**
 * @brief Thin wrapper around mu::Parser for scalar expressions in (t, x, y).
 *
 * This class binds three parser variables (`t`, `x`, `y`) to internal mutable
 * members. Calling operator() updates those values and evaluates the currently
 * stored expression.
 */
class muParserInterface
{
public:
  /// Build an empty parser and bind variables `t`, `x`, `y`.
  muParserInterface();
  /// Build and immediately set the expression.
  muParserInterface(const std::string &s);
  /// Release parser variable bindings.
  ~muParserInterface();
  /// Custom copy: rebuild parser bindings so addresses point to this object.
  muParserInterface(muParserInterface const &);
  /// Custom assignment: same rationale as copy constructor.
  muParserInterface &operator=(muParserInterface const &);

  /// Replace the expression string currently evaluated by muParser.
  void set_expression(const std::string &e);
  /**
   * @brief Evaluate the expression at time t and 2D coordinates.
   *
   * @tparam COORD Any type supporting `coord[0]` and `coord[1]`.
   */
  template <typename COORD>
  double operator()(double const t, COORD const &coord) const;
  /// Convenience overload when coordinates are provided as scalars.
  double operator()(double const t, double const x, double const y) const;

private:
  /// muParser engine evaluating the expression.
  mu::Parser  M_parser;
  /// Mutable because operator() is const but updates bound parser variables.
  mutable double M_t;
  mutable double M_x;
  mutable double M_y;
  /// Local copy of the expression text (used also by copy/assignment).
  std::string M_expr;
};

template <typename COORD>
double
muParserInterface::operator()(double const t, COORD const &coord) const
{
  // Update parser-bound variables before evaluating the expression.
  this->M_t = t;
  this->M_x = coord[0];
  this->M_y = coord[1];
  return this->M_parser.Eval();
}

/// Print detailed muParser exception information.
void printMuException(mu::Parser::exception_type &e);
} // namespace MuParserInterface
#endif
