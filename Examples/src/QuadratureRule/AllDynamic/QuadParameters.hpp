/*
 * QuadParameters.hpp
 *
 *  Created on: Dec 19, 2021
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_QUADRATURERULE_ALLDYNAMIC_QUADPARAMETERS_HPP_
#define EXAMPLES_SRC_QUADRATURERULE_ALLDYNAMIC_QUADPARAMETERS_HPP_
#include <string>
#include <stdexcept>
#include <vector>
#include <iostream>
#include "GetPot"
#include "json.hpp"

namespace apsc::NumericalIntegration
{
  /*!
   * @struct QuadParameters
   * @brief The parameters for numerical quadrature code
   *
   * The same structure provides default values for some of the parameters
   */
  struct QuadParameters
  {
  std::string integrand; // The chosen integrand function
  std::vector<std::string> udflib{"./libudf.so"}; // The integrand libraries to load
  std::vector<std::string> library{"./libAdamsRules.so", "./libGaussRules.so"}; // The quadrature libs
  double a=0; //left point
  double b=1; // right point
  unsigned int  nint=25; // n intervals
  std::string rule="?"; // Chosen rule
  double targetError=1.e-5; // Target error for adaptive stuff
  unsigned int maxIter=100;// Max iteration for adaptive stuff
  };

  QuadParameters readQuadParameters_GP(std::string const & filename);
  QuadParameters readQuadParameters_json(std::string const & filename);


}// end namespace




#endif /* EXAMPLES_SRC_QUADRATURERULE_ALLDYNAMIC_QUADPARAMETERS_HPP_ */
