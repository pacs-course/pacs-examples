/*
 * DecenntDirectionFactory.hpp
 *
 *  Created on: Mar 24, 2022
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_LINESEARCH_DESCENTDIRECTIONFACTORY_HPP_
#define EXAMPLES_SRC_LINESEARCH_DESCENTDIRECTIONFACTORY_HPP_
#include "Factory.hpp"
#include "DescentDirections.hpp"
namespace apsc
{
  /*!
   * The Factory of descent directions. an instance of my generic Factory
   */
  using DescentDirectionFactory=GenericFactory::Factory<DescentDirectionBase,std::string>;
  /*!
   * @brief Load descent directions in the factory.
   *
   * Look at the source code for details. The definition is in the source file.
   * Here, for simplicity, I do not use the constructor attribute to have the automatic registration.
   * The function returns the reference to the only factory in the code.
   */
  DescentDirectionFactory & loadDirections();
  /*!
   * Anothe possibility is to have the factory as global variable
   */
  //extern DescentDirectionFactory & descentDirectionFactory;
}



#endif /* EXAMPLES_SRC_LINESEARCH_DESCENTDIRECTIONFACTORY_HPP_ */
