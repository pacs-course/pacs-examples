/*
 * folding.hpp
 *
 *  Created on: Mar 30, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_TEMPLATES_VARIADICTEMPLATES_FOLDING_HPP_
#define EXAMPLES_SRC_TEMPLATES_VARIADICTEMPLATES_FOLDING_HPP_
#include <iostream>
//! @file  Examples of fold expressions
//!
namespace apsc
{
/*! Checks that all arguments are true
@note I am making it constexpr so that if all argoments are constant expresions
the compiler my compute the result on the fly!
*/
template <typename... Args>
constexpr bool
allTrue(Args const &... args)
{
  return (... && args);
}

//! Make a sum of arguments (when  it makes sense)
/*
@note I am  making it constexpr so that if all argoments are constant expresions
the compiler my compute the result on the fly!
*/
template <typename... Args>
constexpr auto
allSum(Args const &... args)
{
  return (args + ...);
}

//! Compute the average of arguments (if it makes sense)
/*!
@note I am making it constexpr so that if all argoments are constant expresions
the compiler my compute the result on the fly!
*/
template <typename... Args>
constexpr auto
allMean(Args const &... args)
{
  // I cast to double to avoid integer division
  return (args + ...) / static_cast<double>(sizeof...(args));
}

//! Make product of arguments (if it makes sense)
/*!
@note I am making it constexpr so that if all argoments are constant expresions
the compiler my compute the result on the fly!
*/
template <typename... Args>
constexpr auto
allProd(Args const &... args)
{
  return (args * ...);
}
} // end namespace apsc

#endif /* EXAMPLES_SRC_TEMPLATES_VARIADICTEMPLATES_FOLDING_HPP_ */
