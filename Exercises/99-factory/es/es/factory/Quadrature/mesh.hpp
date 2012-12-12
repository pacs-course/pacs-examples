#ifndef _HH_MESH_HH
#define _HH_MESH_HH
#include "domain.hpp"
#include <vector>
  //! functions which generate a 1D mesh
namespace Generator{
  using namespace std;
  vector<double> uniform(double const & a, double const & b, unsigned int const & n);
}
namespace Geometry{
  using std::vector;
  
  /*!  The splitter function defines how the mesh is generated. It takes as
    argument the left and right ends and th enumber of intervals (not node!). It returns the
    vector of nodes. */
  
  typedef std::vector<double> (*Splitter)(double const &,
					  double const &, unsigned int const &);

  
  class Mesh1D{
  public:
    //! constructor takes a domain, the number of intervals and the Splitter
    Mesh1D(Domain1D const & d, unsigned int const &n, Splitter=Generator::uniform);
    inline unsigned int numNodes() const { return myNodes.size();}
    inline double operator [](int i)const {return myNodes[i];}
    inline Domain1D domain() const {return myDomain;}
    double h() const;
  private:
    Domain1D myDomain;
    vector<double> myNodes;
  };

}
#endif
