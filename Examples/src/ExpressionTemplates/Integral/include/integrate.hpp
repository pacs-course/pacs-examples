#ifndef _INTEGRATE_HPP_
#define _INTEGRATE_HPP_

namespace ET
{
/*!
 * @namespace Integrate
 * namespace containing all integration rules
 */
namespace Integrate
{
  /*!
   * @brief mid-point integration rule
   */
  template <typename exprT>
  double
  midPoint(exprT e, double from, double to, int n)
  {
    double sum = 0.;
    double h = (to - from) / n;
    for(int i = 0; i < n; i++)
      {
        double mid = (i + 0.5) * h;
        sum += e.eval(mid) * h;
      }
    return sum;
  }
  /*!
   * @brief mid-point integration rule
   */
  template <typename exprT>
  double
  simpson(exprT e, double from, double to, int n)
  {
    double sum = 0.;
    double h = (to - from) / n;
    for(int i = 0; i < n; i++)
      {
        double left = i * h;
        double mid = left + 0.5 * h;
        double right = left + h;
        sum += (1. / 6) * (4 * e.eval(mid) + e.eval(left) + e.eval(right)) * h;
      }
    return sum;
  }

  /*!
   * @brief trapezoidal integration rule
   */
  template <typename exprT>
  double
  trapez(exprT e, double from, double to, int n)
  {
    double sum = 0.;
    double h = (to - from) / n;
    for(int i = 0; i < n; i++)
      {
        double left = i * h;
        double right = left + h;
        sum += 0.5 * (e.eval(left) + e.eval(right)) * h;
      }
    return sum;
  }

} // namespace Integrate

} // namespace ET

#endif // _INTEGRATE_HPP_
