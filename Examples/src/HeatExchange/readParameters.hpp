#ifndef HH_READPARAMETERS_HH
#define HH_READPARAMETERS_HH
#include <string>
#include "parameters.hpp"
//! Reads problem parameters from GetPot file
/*!
  @param filename The getopot file with the new values
  @param verbose Prints some information on the parameters
 */
parameters readParameters(std::string const &filename, bool verbose = false);
//! Reads problem parameters from json file
/*!
  @param filename The json file with the parameter values
  @param verbose Prints some information on the parameters
 */
parameters readParameters_json(std::string const &filename, bool verbose = false);
#endif
