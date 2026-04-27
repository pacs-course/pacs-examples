/*
 * loadPolyFactory.hpp
 *
 *  Created on: Apr 30, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_FACTORYPLUGIN_LOADPOLYFACTORY_HPP_
#define EXAMPLES_SRC_FACTORYPLUGIN_LOADPOLYFACTORY_HPP_

#include "LoadLibraries.hpp"
namespace Geometry
{
/*!
 * @brief Alias for the dynamic-library loader used by the polygon plugins.
 *
 * The example uses `apsc::LoadLibraries` to open the shared objects listed in
 * `plugins.txt`. When those shared objects are loaded, their namespace-scope
 * registration objects are constructed and populate `polyFactory`.
 */
using loadPolyFactory = apsc::LoadLibraries;
} // namespace Geometry
#endif /* EXAMPLES_SRC_FACTORYPLUGIN_LOADPOLYFACTORY_HPP_ */
