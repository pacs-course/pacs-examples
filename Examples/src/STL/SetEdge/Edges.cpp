#include "Edges.hpp"
#include <algorithm>
#include <array>
  bool yesOrient::operator()(Edge const& a, Edge const & b) const
  {
    // I rely on the fact that the < operator
    // is defined for arrays and does lexicografic comparison
    return a.getPointId()> b.getPointId();
  }

bool noOrient::operator()(Edge const& a, Edge const & b) const
{
  auto amax= std::max(a[0],a[1]);
  auto bmax= std::max(b[0],b[1]);
  if (amax != bmax) 
    return amax>bmax;
    else
      {
	auto amin= std::min(a[0],a[1]);
	auto bmin= std::min(b[0],b[1]);
	return amin > bmin;
      }
  }
