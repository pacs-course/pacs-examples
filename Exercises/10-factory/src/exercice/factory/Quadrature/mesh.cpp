#include <numeric>
#include <algorithm>
#include "mesh.hpp"
namespace Generator
{
  vector<double> uniform(double const & a, double const & b, unsigned int const & n)
  {
    vector<double> mesh(n+1);
    double h=(b-a)/static_cast<double>(n);
    for(int i=0;i<n+1;++i)mesh[i]=a+h*static_cast<double>(i);
    return mesh;

  }
}

namespace Geometry{
  Mesh1D::Mesh1D(Domain1D const & d,  unsigned int const &n, Splitter split):
    myDomain(d),
    myNodes(split(myDomain.left(),myDomain.right(),n)){};

  double Mesh1D::h() const
  { std::vector<double> tmp(myNodes.size());
    std::adjacent_difference(tmp.begin(),tmp.end(),tmp.begin());
    return * std::max_element(tmp.begin()+1,tmp.end());
  }

}
