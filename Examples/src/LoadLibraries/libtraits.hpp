/*
 * lib1.hpp
 *
 *  Created on: Jan 7, 2022
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_LOADLIBRARIES_LIBTRAITS_HPP_
#define EXAMPLES_SRC_LOADLIBRARIES_LIBTRAITS_HPP_
#include <vector>
#include <cmath>
#include <string>
#include "Factory.hpp"
/*!
 * @namespace loadlibraries
 * @brief Facilities used by the dynamically loaded norm examples.
 *
 * The example libraries register functions into a shared factory during the
 * library constructor phase. The main program later queries the factory by
 * name to retrieve the registered callables.
 */
namespace loadlibraries
{
/*!
 * @brief Signature of every function exported through the example plugin
 * factory.
 */
using FunType = std::function<double(std::vector<double> const &)>;

/*!
 * @brief Factory type used to map a string identifier to a registered norm
 * function.
 */
using FunFactory = GenericFactory::FunctionFactory<std::string, FunType>;

/*!
 * @brief Shared singleton factory populated by the dynamically loaded
 * libraries.
 *
 * Entries stored here must be cleared before unloading the libraries, because
 * each stored callable ultimately refers to code that lives inside those
 * shared objects.
 */
extern FunFactory &funFactory;
}


#endif /* EXAMPLES_SRC_LOADLIBRARIES_LIBTRAITS_HPP_ */
