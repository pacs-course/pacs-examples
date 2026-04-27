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
/*!
 * @brief Builder type used to create polygons through the factory.
 *
 * Each registered builder returns ownership of a dynamically created object
 * through `std::unique_ptr<Polygon>`.
 */
using PolyBuilder = std::function<std::unique_ptr<Geometry::Polygon>()>;

//! Identifier type used as the key in the polygon factory.
using PolyId = std::string;

/*!
 * @brief Shared factory mapping polygon names to builder functions.
 *
 * This example uses a plain associative container rather than the more generic
 * `GenericFactory::Factory` abstraction. Plugins populate this map when their
 * shared libraries are loaded.
 */
using PolyFactory = std::map<PolyId, PolyBuilder>;

/*!
 * @brief Global polygon factory shared by the main program and the plugins.
 *
 * The object is defined in `FactoryTraits.cpp` and built into `libFactory.so`
 * so that all loaded plugin libraries refer to the same instance.
 */
extern PolyFactory polyFactory;
} // namespace Geometry

#endif /* EXAMPLES_SRC_FACTORYPLUGIN_FACTORYTRAITS_HPP_ */
