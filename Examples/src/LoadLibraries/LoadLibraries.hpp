/*
 * loadLibraries.hpp
 *
 *  Created on: May 2, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_LOADLIBRARIES_LOADLIBRARIES_HPP_
#define EXAMPLES_SRC_LOADLIBRARIES_LOADLIBRARIES_HPP_
#include <string>
#include <vector>
namespace apsc
{
/*!
 * An utility to load dynamic libraries by name and keep them in a common
 * repository
 *
 */
class LoadLibraries
{
public:
  LoadLibraries() = default;
  //! Constructor that loads libraries from file
  //!
  //! the file should contain the name of the libraries, one for each line
  //! library file names SHOULD NOT contain blanks
  //! @param fileName file containing the name of the libraries to load
  LoadLibraries(std::string fileName) { this->load(fileName); }
  //! loads libraries from a file
  //! the file should contain the name of the libraries, one for each line
  //! @param fileName file containing the name of the libraries to load
  //! @return a bool: if true everything is ok
  bool load(std::string fileName);
  //! Loads a single library given its name and adds it to the repository
  bool loadSingleLibrary(std::string libName);
  //! close all libraries and empties the repo
  void close();

private:
  // The handler of all libraries
  std::vector<void *> loadedLibs;
};

} // namespace apsc

#endif /* EXAMPLES_SRC_LOADLIBRARIES_LOADLIBRARIES_HPP_ */
