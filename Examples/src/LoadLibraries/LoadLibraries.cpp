/*
 * LoadLibraries.cpp
 *
 *  Created on: May 2, 2020
 *      Author: forma
 */
#include "../LoadLibraries/LoadLibraries.hpp"

#include <dlfcn.h>
#include <algorithm>
#include <fstream>
#include <iostream>
namespace apsc
{
bool
LoadLibraries::load(std::string fileName)
{
  bool          good = true;
  std::ifstream pFile(fileName);
  if(pFile.bad())
    {
      std::cerr << " Cannot open plugin files" << std::endl;
      return false;
    }
  while(pFile.good() && (!pFile.eof()) && good)
    {
      std::string line;
      //! get a line
      std::getline(pFile, line);
      // I am assuming no spaces in file name and I find start and end of
      // alphabetic characters This way I account for possible initial or
      // training blanks.
      auto start = find_if(line.begin(), line.end(),
                           [](unsigned char c) { return !std::isblank(c); });
      auto end = find_if(start, line.end(),
                         [](unsigned char c) { return std::isblank(c); });
      if(start != end)
        {
          std::string libName(start, end);
          good = loadSingleLibrary(libName);
        }
      else
        std::clog << "Empty line in plugins file\n";
    }
  return good;
}

bool
LoadLibraries::loadSingleLibrary(std::string libName)
{
  bool good = true;
  // open the dynamic library
  void *sdl_library = dlopen(libName.c_str(), RTLD_NOW);
  // check if error
  if(sdl_library == nullptr)
    {
      std::cerr << " Cannot load Library " << libName << " " << dlerror()
                << std::endl;
      good = false;
    }
  else
    {
      std::clog << libName << " Library opened" << std::endl;
      loadedLibs.push_back(sdl_library);
    }
  return good;
}

void
LoadLibraries::close()
{
  // close all hold libraries
  for(auto l : this->loadedLibs)
    dlclose(l);
  // free vector
  this->loadedLibs.clear();
  this->loadedLibs.shrink_to_fit();
}

} // namespace apsc
