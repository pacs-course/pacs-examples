/*
 * lib1.cpp
 *
 *  Created on: Jan 7, 2022
 *      Author: forma
 */



#include <algorithm>

#include "libtraits.hpp"

namespace
{
  double norm1(std::vector<double> const & x)
  {
    auto res = 0.0;
    for (auto const & y:x) res+=std::abs(y);
    return res;
  }

  double norm0(std::vector<double> const & x)
    {
      return std::count_if(x.begin(), x.end(), [](double const & y){return y==0.;});
    }
}
  __attribute__((constructor))
  void load()
  {
    using namespace loadlibraries;
    funFactory.add("norm1", FunType{norm1});
    funFactory.add("norm0", FunType{norm0});
  }


