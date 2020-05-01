/*
 * FactoryTraits.hpp
 *
 *  Created on: Apr 29, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_FACTORYPLUGIN_FACTORYTRAITS_HPP_
#define EXAMPLES_SRC_FACTORYPLUGIN_FACTORYTRAITS_HPP_
#include <functional>
#include <memory>
#include <string>
#include <map>
#include "Polygon.hpp"
namespace Geometry
{
  //! The way I build a Polygon object
  using PolyBuilder=std::function<std::unique_ptr<Geometry::Polygon>()>;
  //! The identifier used
  using PolyId=std::string;
  //! The factory, just a map!
  using PolyFactory=std::map<PolyId,PolyBuilder>;
  //! The object storing the factory.
  inline PolyFactory polyFactory;
}




#endif /* EXAMPLES_SRC_FACTORYPLUGIN_FACTORYTRAITS_HPP_ */
