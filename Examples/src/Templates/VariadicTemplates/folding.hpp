/*
 * folding.hpp
 *
 *  Created on: Mar 30, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_TEMPLATES_VARIADICTEMPLATES_FOLDING_HPP_
#define EXAMPLES_SRC_TEMPLATES_VARIADICTEMPLATES_FOLDING_HPP_
#include<iostream>
//! @file  Examples of fold expressions
//!
namespace apsc
{
  //! Checks that all arguments are true
  template<typename... Args>
  bool allTrue(Args const &... args)
  {
    return (... && args) ;
  }

  //! Make a sum of arguments (if it makes sense)
  template<typename... Args>
   auto allSum(Args const &... args)
  {
    return (args +...);
  }

  //! Compute the average of arguments (if it makes sense)
  template<typename... Args>
   auto allMean(Args const &... args)
  {
    return (args +...) / sizeof...(args);
  }

  //! Make product of arguments (if it makes sense)
    template<typename... Args>
     auto allProd(Args const &... args)
    {
      return (args *...);
    }
}// end namespace apsc

#endif /* EXAMPLES_SRC_TEMPLATES_VARIADICTEMPLATES_FOLDING_HPP_ */
