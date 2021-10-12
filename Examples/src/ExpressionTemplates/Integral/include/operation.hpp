#ifndef _OPERATION_HPP_
#define _OPERATION_HPP_

namespace ET
{
/*!
 * @namespace Operation
 * this namespace contains all defined operations
 */
namespace Operation
{
  /*!
   * @struct Plus
   * sum
   */
  template <typename T> struct Plus
  {
    T
    operator()(T const &e1, T const &e2) const
    {
      return (e1 + e2);
    }
  };

  /*!
   * @struct Minus
   * difference
   */
  template <typename T> struct Minus
  {
    T
    operator()(T const &e1, T const &e2) const
    {
      return (e1 - e2);
    }
  };

  /*!
   * @struct Multiply
   * multiplication
   */
  template <typename T> struct Multiply
  {
    T
    operator()(T const &e1, T const &e2) const
    {
      return (e1 * e2);
    }
  };

  /*!
   * @struct Divide
   * division
   */
  template <typename T> struct Divide
  {
    T
    operator()(T const &e1, T const &e2) const
    {
      return (e1 / e2);
    }
  };

  /*!
   * @struct Exp
   * exponential
   */
  template <typename T> struct Exp
  {
    T
    operator()(T const &e) const
    {
      return std::exp(e);
    }
  };

} // namespace Operation

} // namespace ET

#endif // _OPERATION_HPP_
