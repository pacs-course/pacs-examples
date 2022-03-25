/*
 * DescentDirectionFactory.cpp
 *
 *  Created on: Mar 24, 2022
 *      Author: forma
 */
#include "DescentDirectionFactory.hpp"
namespace apsc
{
DescentDirectionFactory &
loadDirections()
{
  // get the factory
  DescentDirectionFactory& theFactory =  DescentDirectionFactory::Instance();
  //
  theFactory.add("GradientDirection",[](){return std::make_unique<GradientDirection>();});
  theFactory.add("BFGSDirection",[](){return std::make_unique<BFGSDirection>();});
  theFactory.add("BFGSIDirection",[](){return std::make_unique<BFGSIDirection>();});
  theFactory.add("BBDirection",[](){return std::make_unique<BBDirection>();});
  theFactory.add("CGDirection",[](){return std::make_unique<CGDirection>();});
  return theFactory;
}
}
