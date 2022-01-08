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
* @brief Some functions just to show how the loadlibrary utility works
 *
 */
namespace loadlibraries
{
using FunType=std::function<double (std::vector<double> const &)>;
using FunFactory=GenericFactory::FunctionFactory<std::string,FunType>;
extern FunFactory& funFactory; // The factory as an external object
}


#endif /* EXAMPLES_SRC_LOADLIBRARIES_LIBTRAITS_HPP_ */
