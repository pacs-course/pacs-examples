//
// Created by forma on 03/04/23.
//

#ifndef EXAMPLES_BOOLEANCONCEPT_HPP
#define EXAMPLES_BOOLEANCONCEPT_HPP

namespace apsc
{
namespace TypeTraits
{
  /*!
   * A simple concept to test if a type conforms to a boolean.
   *
   * Taken from "A tour of C++" by B. Stroustrup, II edition.
   * @tparam B The type to test
   */
  template <typename B>
  concept Boolean = requires(B x, B y) {
    {x = true}; // assigning true is valid
    {x = false};// assigning false is valid
    {x = (x == y)};// equivalence returns something that can be assigned
    {x = (x != y)};// in-equivalence returns something that can be assigned
    {x = !x};    // negation returns something that can be assigned
    {x = (x = y)};// assignment returns something that can be assigned
  };
} // end TypeTraits
} // end apsc
#endif // EXAMPLES_BOOLEANCONCEPT_HPP
