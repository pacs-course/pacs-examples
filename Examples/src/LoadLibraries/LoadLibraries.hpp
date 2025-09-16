/*
 * loadLibraries.hpp
 *
 *  Created on: May 2, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_LOADLIBRARIES_LOADLIBRARIES_HPP_
#define EXAMPLES_SRC_LOADLIBRARIES_LOADLIBRARIES_HPP_
#include <dlfcn.h>
#include <string>
#include <unordered_map>

namespace apsc
{
/*!
 * @brief An utility to load dynamic libraries by name and keep them in a common
 * repository
 *
 * The object of this class must be in the accessible for all the time the
 * libraries are used. Indeed it applies the RAII principle, and the descructor
 * closes the libraries. When a dynamic library is closed the symbols it
 * provides are not accessible anymore.
 *
 */
class LoadLibraries
{
public:
  LoadLibraries() = default;
  //! This class can be moved but not copied
  LoadLibraries(LoadLibraries &&) = default;
  //! This class can be moved but not copied
  LoadLibraries &operator=(LoadLibraries &&) = default;
  //! This class is not copy-assignable
  LoadLibraries &operator=(LoadLibraries const &) = delete;
  //! This class is not copy-constructible
  LoadLibraries(LoadLibraries const &) = delete;
  //! Constructor that loads libraries from file
  //!
  //! the file should contain the name of the libraries, one for each line
  //! library file names SHOULD NOT contain blanks
  //! @param fileName file containing the name of the libraries to load
  //! @param mode the mode to load the libraries. Default is RTLD_NOW, all
  //! symbols are resolved upfront. Use instead RTLD_LAZY if you want to resolve
  //! the symbols only
  //!              when needed
  explicit LoadLibraries(std::string const &fileName, int mode = RTLD_NOW)
  {
    this->load(fileName, mode);
  }
  //! loads libraries from a file
  //! the file should contain the name of the libraries, one for each line
  //! @param fileName file containing the name of the libraries to load
  //! @param mode the mode to load the libraries. Default is RTLD_NOW, all
  //! symbols are resolved upfront. Use instead RTLD_LAZY if you want to resolve
  //! the symbols only
  //!              when needed
  //! @return a bool: if true everything is ok
  bool load(std::string fileName, int mode = RTLD_NOW);
  //! Loads a single library given its name and adds it to the repository
  bool loadSingleLibrary(std::string libName, int mode = RTLD_NOW);
  //! closes all libraries and empties the container
  void close();
  //! closes a specific library. If library not present, it is a NoOp.
  void close(std::string libname);
  //! Gets the handle to a specific library. Returns nullptr if library not
  //! present.
  void *getLibraryHandle(std::string libName) const;
  ~LoadLibraries() { this->close(); } //! destructor closes the libraries

private:
  // The handler of all libraries
  std::unordered_map<std::string, void *> loadedLibs;
};

} // namespace apsc

#endif /* EXAMPLES_SRC_LOADLIBRARIES_LOADLIBRARIES_HPP_ */
