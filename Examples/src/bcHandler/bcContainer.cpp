/*
 * bc_factory1.cpp
 *
 *  Created on: Nov 28, 2008
 *      Author: forma
 */
#include "bcContainer.hpp"
#include <algorithm>

namespace FEM
{
  std::vector<BCContainer::const_iterator> extractBCWithType(BCContainer const & bcContainer,BCType const & type)
  {
    isBCTypeEqual predicate(type);
    std::vector<BCContainer::iterator> tmp;
    for(auto i=bcContainer.begin();i<bcContainer.end();++i)
    {
      if (predicate(*i)) tmp.push_back(i);
    }
    return tmp;
  }

  std::vector<BCContainer::const_iterator> extractBCWithName(BCContainer const & bcContainer,BCName const & type){
    isBCNameEqual predicate(type);
    std::vector<BCContainer::iterator> tmp;
    for(auto i=bcContainer.begin();i<bcContainer.end();++i)
      if(predicate(*i))tmp.push_back(i);
    return tmp;
  }
  
}





