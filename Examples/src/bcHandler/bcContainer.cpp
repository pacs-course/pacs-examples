/*
 * bc_factory1.cpp
 *
 *  Created on: Nov 28, 2008
 *      Author: forma
 */
#include "bcContainer.hpp"
#include <algorithm>

namespace apsc::FEM
{
std::vector<BCBase>
extractBCWithType(BCContainer const &bcContainer, BCType const &type)
{
  isBCTypeEqual       predicate(type);
  std::vector<BCBase> tmp;
  for(auto const &i : bcContainer)
    {
      if(predicate(i))
        tmp.push_back(i);
    }
  return tmp;
}

std::vector<BCBase>
extractBCWithName(BCContainer const &bcContainer, BCName const &type)
{
  isBCNameEqual       predicate(type);
  std::vector<BCBase> tmp;
  for(auto const &i : bcContainer)
    if(predicate(i))
      tmp.push_back(i);
  return tmp;
}

} // namespace apsc::FEM
