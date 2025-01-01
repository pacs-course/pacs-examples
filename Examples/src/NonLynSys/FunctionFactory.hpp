/*
 * FunctionFactory.hpp
 *
 *  Created on: Feb 13, 2021
 *      Author: forma
 */
/*!
 * @file FunctionFactory.hpp
 *
 * Here we show how the utility in NonLynSys may be used to build Function
 * Factory: an object factory where the objects are in fact callable objects
 */
#include "NonLinSys.hpp"
#include <exception>
#include <map>
#include <type_traits>
namespace apsc
{
/*! @ brief a function factory

 * @details This is an example of a factory of functions built on top of
 NonLinSys.
 * You can add callable objects that respect the argument and return type
 specified
 * in the given trait, with an associated identifier (here given as a string).
 * You can the retrieve the function corresponding to that identifier.
 *
 * @note it is an example of protected inheritance. I inherit from NonLinSys
 * since the factory is an extension of it, but I do not want to expose all
 * methods of NonLinSys, in particular the method addToSystem. Otherwise
 * I could add a function without identifier!.
 * @tparam R The scalar type
 * @tparam Traits The traits containing the main types used
 */
template <typename R = double, template <typename> typename Traits =
                                 apsc::NonLinSysTraits::VectorTraits>
class FunctionFactory : protected apsc::NonLinSys<R, Traits>
{
public:
  //! The methods of NonLinSys I want expose to the public
  using apsc::NonLinSys<R, Traits>::numEqs;
  //! for compatibility with an earlier version
  auto
  size()
  {
    return numEqs();
  }
  /*!
   * @brief adds a function to the factory
   *
   * It throws an exception if the identifier is already taken by another
   * function
   *
   * @tparam FUN The type of a functor compliant with the Trait
   * @param s A string used as identifier of the function
   * @param f The function to store. It can be any callable object complieant
   * with the trait
   */
  template <typename FUN>
  void
  addToFactory(std::string const &s, FUN &&f)
  {
    using namespace std::string_literals;
    static_assert(
      std::is_convertible_v<FUN, typename Traits<R>::ScalarFunctionType>,
      "Cannot add a function with wrong signature!");
    //  add the identifier
    auto [it, success] = functionPos_.insert({s, numEqs()});
    if(!success)
      {
        // Note the use of the string literal to be able to use +. Alternative:
        // create a string with std::string
        throw std::runtime_error(
          "Cannot insert function with same identifier twice. Identifier= "s +
          s);
      }
    // Add the function
    // Here I need this->, since I am in a template method and I have to recall
    // a base method not publicly accessible! Alternative: use the full
    // qualified name; apsc::NonLinSys<R, Traits>::addToSystem
    this->addToSystem(std::forward<FUN>(f));
  }

  /*!
   * @brief Get a function from the factory
   * It throws an exception if the function is not found
   * @param s the string
   * @return the function (a callable object of the type specified in the
   * Trait).
   */
  auto
  getFunction(std::string const &s) const
  {
    using namespace std::string_literals;
    auto it = functionPos_.find(s);
    if(it != functionPos_.end())
      {
        return apsc::NonLinSys<R, Traits>::getFunction(it->second);
      }
    else
      {
        // note the string literal s to be able to use +
        throw std::runtime_error("Cannot find function with identifier="s + s);
      }
  }

private:
  // The dictionary map -> position in the vector stored in NonLinSys
  std::map<std::string, std::size_t> functionPos_;
};
} // namespace apsc

#ifndef EXAMPLES_SRC_NONLYNSYS_FUNCTIONFACTORY_HPP_
#define EXAMPLES_SRC_NONLYNSYS_FUNCTIONFACTORY_HPP_

#endif /* EXAMPLES_SRC_NONLYNSYS_FUNCTIONFACTORY_HPP_ */
