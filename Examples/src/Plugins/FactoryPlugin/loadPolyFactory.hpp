/*
 * loadPolyFactory.hpp
 *
 *  Created on: Apr 30, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_FACTORYPLUGIN_LOADPOLYFACTORY_HPP_
#define EXAMPLES_SRC_FACTORYPLUGIN_LOADPOLYFACTORY_HPP_
#include "FactoryTraits.hpp"
#include <dlfcn.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
namespace Geometry
{
/*!
 * A class that loads, opens and holds dynamic libraries whose name is stored in
 * a file
 */
class loadPolyFactory
{
public:
  loadPolyFactory() = default;
  //! Constructor that loads libraries
  loadPolyFactory(std::string s) { this->load(s); }
  //! loads load library names (plugins) from file
  bool load(std::string pluginFile);
  //! close libraries
  void close();

private:
  std::vector<void *> loadedLibs;
};

} // namespace Geometry

#endif /* EXAMPLES_SRC_FACTORYPLUGIN_LOADPOLYFACTORY_HPP_ */
