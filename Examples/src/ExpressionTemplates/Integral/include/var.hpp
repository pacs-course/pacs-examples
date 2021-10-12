#ifndef _VAR_HPP_
#define _VAR_HPP_

namespace ET
{
/*! @namespace Var
 * namespace for the variables, defined as simple structs that store the
 * corresponding value
 */

namespace Var
{
  /*! @class Literal
   * this class models a literal variable, such as constants */
  struct Literal
  {
    //! @brief constructor that sets the Literal to M_val
    Literal(double val) : M_val(val) {}

    //! @brief return the value of the Literal
    double
    eval(double /*dummy*/) const
    {
      return M_val;
    }

  private:
    const double M_val;
  };

  /*! @class Identity
   * class that represents the 'x' variable.
   * this class does not need to store a value, since it returns always the
   * current value
   */
  template <typename T = double> struct Identity
  {
    //! @brief return the value that is passed
    T
    eval(T val) const
    {
      return val;
    }
  };

  /*!
   * @class exprTraits
   * this structs are traits to map pod types to Var namespace correspondences
   */
  template <class exprT> struct exprTraits
  {
    typedef exprT expr_Type;
  };
  template <> struct exprTraits<double>
  {
    typedef Literal expr_Type;
  };
  // template <>            struct exprTraits<int>    { typedef Literal
  // expr_Type; };

} // namespace Var

} // namespace ET

#endif // _VAR_HPP_
