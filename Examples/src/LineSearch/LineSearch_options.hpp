/*
 * LineSearch_options.hpp
 *
 *  Created on: Dec 27, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_LINESEARCH_LINESEARCH_OPTIONS_HPP_
#define EXAMPLES_SRC_LINESEARCH_LINESEARCH_OPTIONS_HPP_
#include "LineSearch_traits.hpp"
#include "json.hpp"
#include <fstream>
#include <string>
namespace apsc
{
struct LineSearchOptions
{
  using Scalar = apsc::LineSearch_traits::Scalar;
  Scalar sufficientDecreaseCoefficient =
    1.e-2; //!< The coefficient for sufficient decrease
  Scalar stepSizeDecrementFactor = 0.5; //!< How much to decrement alpha
  Scalar secondWolfConditionFactor =
    0.9;                          //!< Second Wolfe conditin factor (not used)
  Scalar       initialStep = 1.0; //!< the initial apha value.
  unsigned int maxIter =
    40; //!< max number of iterations in the backtracking algorithm
  std::string descentDirection =
    "GradientDirection"; //!< The descent direction to use
  //! Read options from a JSON file.
  //! Expected keys (all optional): sufficientDecreaseCoefficient,
  //! stepSizeDecrementFactor, secondWolfConditionFactor, initialStep, maxIter,
  //! descentDirection.
  void
  readFromFile(const std::string &filename)
  {
    std::ifstream file(filename);
    if(!file.is_open())
      throw std::runtime_error("LineSearchOptions: cannot open file " +
                               filename);
    nlohmann::json j;
    file >> j;
    if(j.contains("sufficientDecreaseCoefficient"))
      sufficientDecreaseCoefficient =
        j["sufficientDecreaseCoefficient"].get<Scalar>();
    if(j.contains("stepSizeDecrementFactor"))
      stepSizeDecrementFactor = j["stepSizeDecrementFactor"].get<Scalar>();
    if(j.contains("secondWolfConditionFactor"))
      secondWolfConditionFactor = j["secondWolfConditionFactor"].get<Scalar>();
    if(j.contains("initialStep"))
      initialStep = j["initialStep"].get<Scalar>();
    if(j.contains("maxIter"))
      maxIter = j["maxIter"].get<unsigned int>();
    if(j.contains("descentDirection"))
      descentDirection = j["descentDirection"].get<std::string>();
  }
  /*!
  \brief streaming operator that prints the current options to a stream
  */
  friend std::ostream &
  operator<<(std::ostream &os, const LineSearchOptions &options)
  {
    os << "LineSearchOptions:\n";
    os << "  sufficientDecreaseCoefficient: "
       << options.sufficientDecreaseCoefficient << "\n";
    os << "  stepSizeDecrementFactor: " << options.stepSizeDecrementFactor
       << "\n";
    os << "  secondWolfConditionFactor: " << options.secondWolfConditionFactor
       << "\n";
    os << "  initialStep: " << options.initialStep << "\n";
    os << "  maxIter: " << options.maxIter << "\n";
    os << "  descentDirection: " << options.descentDirection << "\n";
    return os;
  }
};
} // namespace apsc

#endif /* EXAMPLES_SRC_LINESEARCH_LINESEARCH_OPTIONS_HPP_ */
