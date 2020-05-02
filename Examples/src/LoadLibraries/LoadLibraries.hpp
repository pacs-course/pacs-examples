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
   * An utility to load dynamic libraries read from text file
   *
   * The text file should contain the name of the libraries, one at a time.
   */
  class LoadLibraries
  {
  public:
      LoadLibraries()=default;
      //! Constructor that loads libraries from file
      //! @param fileName file containing the name of the libraries to load
      LoadLibraries(std::string fileName){this->load(fileName);}
      //! loads load library names (plugins) from file
      //! @param fileName file containing the name of the libraries to load
      //! @return a bool: if true everything is ok
      bool load(std::string fileName);
      //! close libraries
      void close();
    private:
      std::vector<void *> loadedLibs;
  };


}




#endif /* EXAMPLES_SRC_LOADLIBRARIES_LOADLIBRARIES_HPP_ */
