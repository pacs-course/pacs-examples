/*
 * LoadLibraries.cpp
 *
 *  Created on: May 2, 2020
 *      Author: forma
 */
#include "LoadLibraries.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
namespace apsc
{
bool
LoadLibraries::load(std::string fileName, int mode)
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
          good = loadSingleLibrary(libName, mode);
        }
      else
        std::clog << "Empty line in plugins file\n";
    }
  return good;
}

bool
LoadLibraries::loadSingleLibrary(std::string libName, int mode)
{
  bool good = true;
  auto found = (loadedLibs.find(libName) != loadedLibs.end());
  if(found)
    {
      std::cerr << libName << "WARNING: Library " << libName
                << " already existing."
                << "Cannot open the same library twice" << std::endl;
    }
  else
    {
      // open the dynamic library
      void *sdl_library = dlopen(libName.c_str(), mode);
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
          loadedLibs.insert({libName, sdl_library});
        }
    }
  return good;
}

void
LoadLibraries::close()
{
  // close all held libraries
  for(auto [_, l] : this->loadedLibs)
    dlclose(l);
  // free map
  this->loadedLibs.clear();
}

void
LoadLibraries::close(std::string libName)
{
  auto found = loadedLibs.find(libName);
  if(found != loadedLibs.end())
    {
      dlclose(found->second);
      loadedLibs.erase(found);
    }
  // if lib not present is a NoOp.
}

void *
LoadLibraries::getLibraryHandle(std::string libName) const
{
  auto found = loadedLibs.find(libName);
  if(found == loadedLibs.end())
    {
      return nullptr;
    }
  else
    {
      return found->second;
    }
}

} // namespace apsc
