/*
 * FactoryTraits.hpp
 *
 *  Created on: Apr 29, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_FACTORYPLUGIN_FACTORYTRAITS_HPP_
#define EXAMPLES_SRC_FACTORYPLUGIN_FACTORYTRAITS_HPP_
#include "Polygon.hpp"
#include <functional>
#include <map>
#include <memory>
#include <string>
namespace Geometry
{
//! The way I build a Polygon object
using PolyBuilder = std::function<std::unique_ptr<Geometry::Polygon>()>;
//! The identifier used
using PolyId = std::string;
//! The factory, just a map!
using PolyFactory = std::map<PolyId, PolyBuilder>;
//! The object storing the factory. Defined in the source file (must be stored
//! in a shared lib)
extern PolyFactory polyFactory;
} // namespace Geometry

#endif /* EXAMPLES_SRC_FACTORYPLUGIN_FACTORYTRAITS_HPP_ */
