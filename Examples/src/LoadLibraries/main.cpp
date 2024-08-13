/*
 * main.cpp
 *
 *  Created on: Jan 7, 2022
 *      Author: forma
 */
#include "LoadLibraries.hpp"
#include "libtraits.hpp"
#include <iostream>

int
main()
{
  using namespace loadlibraries;
  using apsc::LoadLibraries;
  LoadLibraries libraries("libraries.txt");
  std::cout << "Libraries loaded" << std::endl;
  auto registeredFunctions = funFactory.registered();
  std::cout << "Obtained " << registeredFunctions.size()
            << " registered functions" << std::endl;
  std::vector x{1., 0., 3., 4., 5., 6., -9.};
  for(auto fun : registeredFunctions)
    {
      std::cout << "Obtaining registerd function: " << fun << std::endl;
      FunType f;
      f = funFactory.get(fun);
      auto result = f(x);
      std::cout << fun << "(x)=" << result << std::endl;
    }
  // I need first to clear the factory of functions, since they are
  // linked to the library!
  // Failure to do so creates a memory leak, since the pointer strored in the
  // function wrapper used by funFactory is a pointer to sybols pprovided
  // by the libraries in libraries.
  funFactory.clear();
  // now close libraries
  libraries.close();
}
