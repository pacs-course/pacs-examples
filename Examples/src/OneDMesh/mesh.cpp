#include<numeric>
#include<algorithm>
#include "mesh.hpp"
namespace Geometry{
  Mesh1D::Mesh1D(Domain1D const & d,  unsigned int const &n):
    myDomain(d)
  {
    Uniform g(d,n);
    myNodes=g();
  };
  
  double Mesh1D::h() const
  { std::vector<double> tmp(myNodes.size()-1);
    std::adjacent_difference(myNodes.begin(),myNodes.end(),tmp.begin());
    return * std::max_element(tmp.begin(),tmp.end());
  }

  void  Mesh1D::reset(OneDMeshGenerator const & mg){
    myDomain=mg.getDomain();
    myNodes=mg();
  }
  
}
