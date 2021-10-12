#ifndef HH_TWOFUNCTIONS_HH
#define HH_TWOFUNCTIONS_HH
/*! \file twofunctions.hpp
  An example of use of constexpr functions
*/

//! A pure declaration of a normal function
double cube(double const &x) noexcept; // a pure declaration

/*!
  The definition of a constexpr function must be in the header file
  since a constexpr function is implicitly an inline function
 */
constexpr double
cubeconst(double x) noexcept
{
  return x * x * x;
}

#endif
