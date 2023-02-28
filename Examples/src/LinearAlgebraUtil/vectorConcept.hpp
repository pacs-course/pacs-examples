//
// Created by forma on 24/02/23.
//

#ifndef EXAMPLES_VECTORCONCEPT_H
#define EXAMPLES_VECTORCONCEPT_H
#if __cplusplus < 202002L
#error You must use at least c++20
#else
#include <concepts>
#include "is_complex.hpp"
namespace apsc
{
namespace TypeTraits
{
template <class T>
concept AlgebraicVector = requires(T v)
{
  {v[0]};
  {v[0]+v[0]};
  {v[0]/v[0]};
  {v[0]*v[0]};
  {v[0]-v[0]};

};
} // End Typetraits
} // end apsc
#endif

#endif // EXAMPLES_VECTORCONCEPT_H
