/*
 * loadLibraries.hpp
 *
 *  Created on: May 2, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_LOADLIBRARIES_LOADLIBRARIES_HPP_
#define EXAMPLES_SRC_LOADLIBRARIES_LOADLIBRARIES_HPP_
#include <dlfcn.h>
#include <ranges>
#include <string>
#include <unordered_map>

namespace apsc
{
/*!
 * @brief Loads shared libraries and keeps their handles alive for a controlled
 * lifetime.
 *
 * The class owns the handles returned by `dlopen()` and releases them in the
 * destructor through `dlclose()`. Any symbol or object obtained from a loaded
 * library becomes invalid as soon as the corresponding handle is closed.
 * Therefore a `LoadLibraries` instance must outlive every object that depends
 * on code or data defined in the loaded libraries.
 */
class LoadLibraries
{
public:
  //! Builds an empty repository of loaded libraries.
  LoadLibraries() = default;
  //! Moves the repository, transferring ownership of every loaded handle.
  LoadLibraries(LoadLibraries &&) = default;
  //! Moves the repository, transferring ownership of every loaded handle.
  LoadLibraries &operator=(LoadLibraries &&) = default;
  //! Copy assignment is disabled because the class uniquely owns library
  //! handles.
  LoadLibraries &operator=(LoadLibraries const &) = delete;
  //! Copy construction is disabled because the class uniquely owns library
  //! handles.
  LoadLibraries(LoadLibraries const &) = delete;
  /*!
   * @brief Builds the repository and immediately loads libraries from a file.
   *
   * The input file is interpreted as one library name per line. Leading and
   * trailing blanks are ignored, while embedded blanks terminate the library
   * name because the parser extracts the first blank-separated token.
   *
   * @param fileName Path to the file containing the libraries to load.
   * @param mode Dynamic loader mode forwarded to `dlopen()`.
   */
  explicit LoadLibraries(std::string const &fileName, int mode = RTLD_NOW)
  {
    this->load(fileName, mode);
  }
  /*!
   * @brief Loads all libraries listed in a text file.
   *
   * The file is read sequentially and loading stops on the first failure.
   * Empty or blank-only lines are ignored.
   *
   * @param fileName Path to the file containing the libraries to load.
   * @param mode Dynamic loader mode forwarded to `dlopen()`.
   * @return `true` if every requested library was loaded successfully,
   * `false` otherwise.
   */
  bool load(std::string fileName, int mode = RTLD_NOW);
  /*!
   * @brief Loads one shared library and stores its handle.
   *
   * If the same library name is already present, the repository is left
   * unchanged.
   *
   * @param libName Library path or soname accepted by `dlopen()`.
   * @param mode Dynamic loader mode forwarded to `dlopen()`.
   * @return `true` on success, `false` if `dlopen()` fails.
   */
  /*
  @brief Load libraries from a Range of strings
  */
  bool
  loadLibs(std::ranges::input_range auto &&libNames, int mode = RTLD_NOW)
  {
    bool good = true;
    for(auto &&libName : libNames)
      {
        if(libName.empty())
          {
            good = false;
            break;
          }
        good = this->loadSingleLibrary(libName, mode);
        if(!good)
          break;
      }
    return good;
  }

  bool loadSingleLibrary(std::string libName, int mode = RTLD_NOW);
  //! Closes every loaded library and clears the internal repository.
  void close();
  /*!
   * @brief Closes a specific library and removes it from the repository.
   * @param libname Library name used as the key in the repository.
   */
  void close(std::string libname);
  /*!
   * @brief Returns the handle associated with a loaded library.
   * @param libName Library name used as the key in the repository.
   * @return The `dlopen()` handle, or `nullptr` if the library is not loaded.
   */
  void *getLibraryHandle(std::string libName) const;
  //! Closes every loaded library owned by the repository.
  ~LoadLibraries() { this->close(); }

private:
  //! Maps library names to the raw handles returned by `dlopen()`.
  std::unordered_map<std::string, void *> loadedLibs;
};

} // namespace apsc

#endif /* EXAMPLES_SRC_LOADLIBRARIES_LOADLIBRARIES_HPP_ */
