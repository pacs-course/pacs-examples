/*
 * loadPolyFactory.hpp
 *
 *  Created on: Apr 30, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_FACTORYPLUGIN_LOADPOLYFACTORY_HPP_
#define EXAMPLES_SRC_FACTORYPLUGIN_LOADPOLYFACTORY_HPP_
#include "FactoryTraits.hpp"
#include <iostream>
#include <dlfcn.h>
#include <string>
#include <fstream>
#include <vector>
#include<algorithm>
namespace Geometry
{
  /*!
   * A class that loads, opens and holds dynamic libraries whose name is stored in a file
   */
  class loadPolyFactory
  {
  public:
    loadPolyFactory()=default;
    //! Constructor that loads libraries
    loadPolyFactory(std::string s){this->load(s);}
    //! loads load library names (plugins) from file
    bool load(std::string pluginFile);
    //! close libraries
    void close();
  private:
    vector<void *> loadedLibs;
  };

}



#endif /* EXAMPLES_SRC_FACTORYPLUGIN_LOADPOLYFACTORY_HPP_ */
