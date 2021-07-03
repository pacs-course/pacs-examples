/*
 * FunctionFactory.hpp
 *
 *  Created on: Feb 13, 2021
 *      Author: forma
 */
/*!
 * @file FunctionFactory.hpp
 *
 * Here we show how the utility in NonLynSys may be used to build Function Factory:
 * an object factory where the objects are in fact callable objects
 */
#include "NonLinSys.hpp"
#include <map>
#include <exception>
namespace apsc
{
  /*!
   * This is an example of a factory of functions You can add callable objects that respect the argument and
   * return type specified in the given trait, with an associated identifier (here given as a string).
   * You can the retrieve the function corresponding to that identifier.
   *
   * @note it is an example of protected inheritance. I inherit from NonLinSys since the factory is an extension of it,
   * but I do not want to expose all methods of NonLinSys, in particular the method addToSystem. Indeed, otherwise
   * it could add a function without identifier!.
   * @tparam R The scalar type
   * @tparam Traits The traits containing the main types used
   */
  template<typename R, template<typename> typename Traits=apsc::NonLinSysTraits::VectorTraits >
   class FunctionFactory : protected apsc::NonLinSys<R,Traits>
    {
    public:
    //! The methods of NonLinSys I want expose to the public
    using apsc::NonLinSys<R,Traits>::numEqs;
    //! for compatibility with earlier version
    auto size(){return numEqs();}
    // Overloading a base function
    template <typename scalarfunction>
    void addToFactory(std::string const & s,scalarfunction&& f)
    {
      apsc::NonLinSys<R,Traits>::addToSystem(std::forward<scalarfunction>(f));
      auto [it,success] = functionPos_.insert({s,numEqs()-1u});
      if(!success)
        {
          throw std::runtime_error(
              std::string{"Cannot insert function with same identifier twice. Identifier="}+ s
              );
        }
    }

    auto getFunction(std::string const & s) const
    {
      auto it = functionPos_.find(s);
      if (it !=functionPos_.end())
        {
          return  apsc::NonLinSys<R,Traits>::getFunction(it->second);
        }
      else
        {
          throw std::runtime_error(
              std::string{"Cannot find function with identifier="}+ s
              );
        }
    }
    private:
    // The dictionary map -> position in the vector stored in NonLinSys
    std::map<std::string, std::size_t> functionPos_;
    };
}


#ifndef EXAMPLES_SRC_NONLYNSYS_FUNCTIONFACTORY_HPP_
#define EXAMPLES_SRC_NONLYNSYS_FUNCTIONFACTORY_HPP_





#endif /* EXAMPLES_SRC_NONLYNSYS_FUNCTIONFACTORY_HPP_ */
