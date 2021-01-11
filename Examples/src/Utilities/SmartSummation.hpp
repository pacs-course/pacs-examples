/*
 * SmartSummation.hpp
 *
 *  Created on: Jan 4, 2021
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_SMARTSUMMATION_SMARTSUMMATION_HPP_
#define EXAMPLES_SRC_SMARTSUMMATION_SMARTSUMMATION_HPP_
#include <vector>
#include <algorithm>
#include <functional>
#include <cmath>
#include <type_traits>
namespace apsc
{
  /*!
   * CompensatedSummation uses the Kahan algorithm to reduce round-off error when summing
   * the elements of a given sequence.
   * For more information consult Section 4.3 of  N.J. Higham, Accuracy and Stability of Numerical Algorithms, SIAM, 1996
   *
   * @tparam ForwardIt A forward iterator to a sequance storing floating point numbers
   * @param start The beginning of the sequence
   * @param finish The end of the sequence
   * @return The sum of the element of the sequence
   */
  template <typename ForwardIt>
  auto CompensatedSummation(ForwardIt const & start, ForwardIt const & finish)
  {
    // Scalar is the type of the objects stored in the range. I get the info on the type dereferencing the iterator.
    // dereferencing an iterator however returns a reference, maybe a const reference, so I strip
    // away references and const from the type
    // With low precision floatings point types (like float) it has better take the variables used to
    // store the sum and the temporaris as double. But is a little cheating! So I comment that line.
    //using Scalar = std::common_type_t<double, std::remove_cv_t<std::remove_reference_t<decltype(*start)>>>;
    using Scalar = std::remove_cv_t<std::remove_reference_t<decltype(*start)>>;
    Scalar volatile s=0;
    Scalar e=0;
    for (auto x = start; x!=finish; ++x)
      {
        // Declared volatile to avoid too smart compiler optimizations!
        auto volatile temp = s;
        auto volatile y = *x + e;
        s = temp+y;
        e = (temp -s)+y;
      }
    s+=e;
    return s;
  }
  /*!
   * DoublyCompensatedSummation uses the double compensatio algorithm to reduce round-off error when summing
   * the elements of a given sequence.
   * For more information consult Section 4.3 of  N.J. Higham, Accuracy and Stability of Numerical Algorithms, SIAM, 1996
   *
   * @tparam ForwardIt A forward iterator to a sequance storing floating point numbers
   * @param start The beginning of the sequence
   * @param finish The end of the sequence
   * @return The sum of the element of the sequence
   */
  template <typename ForwardIt>
  auto DoublyCompensatedSummation(ForwardIt const & start, ForwardIt const & finish)
  {
    // Scalar is the type of the objects stored in the range. I get the info on the type dereferencing the iterator.
    // dereferencing an iterator however returns a reference, maybe a const reference, so I strip
    // away references and const from the type
    using Scalar = std::remove_cv_t<std::remove_reference_t<decltype(*start)>>;

    std::vector<Scalar> v(start,finish);
    std::sort(v.begin(),v.end(),[](Scalar const & a, Scalar const & b){return std::abs(a)>std::abs(b);});
    Scalar volatile s=v[0];
    Scalar c=0;
    for (auto x = v.begin()+1; x!= v.end(); ++x)
      {
        auto volatile y = c + *x;
        auto volatile u = *x - (y-c);
        auto volatile t = y + s;
        auto volatile v = y - (t-s);
        auto volatile z = u + v;
        s      = t + z;
        c      = z - (s-t);
      }
    return s;
  }
}// end namespace apsc



#endif /* EXAMPLES_SRC_SMARTSUMMATION_SMARTSUMMATION_HPP_ */
