#ifndef _HH_MESH_HH
#define _HH_MESH_HH
#include "domain.hpp"
#include <vector>
#include <functional>
namespace Generator{
  /*! \defgroup meshers Functions which generate a 1D mesh.
    @{ */
  using namespace std;
  vector<double> uniform(double const & a, double const & b, 
			 unsigned int const & n);
  /*! @}*/
}

namespace Geometry{
  using std::vector;
  /*!  
    Type of function which generates a mesh.
    
    The splitter function defines how the mesh is generated. It
    takes as argument the left and right ends and the number of
    intervals (not node!). It returns the vector of nodes. 
  */
  
  typedef std::function<std::vector<double>(double const &, double const &, 
					    unsigned int const &)> Splitter;
  
class Mesh1D{
public:
  //! Default constructor is deleted.
  /*!  Since I have not implemented methods that build the mesh on an
    existing Mesh1D object (the mesh is built in the non-default
    construtor) It makes no sense to have a default constructor.
    
    \note In fact, since I have another constructor, there is no need
    to explicitly indicate that the default is deleted. Yet for clarity
    is better to say it.
   */
  Mesh1D()=delete;
  //! Constructor 
  /*! \param d  A domain
      \param n  Number of intervals (not nodes!)
      \param s  The Splitter used to generate the mesh.
  */
  Mesh1D(Domain1D const & d, unsigned int const &n, 
	 Splitter s=&Generator::uniform);
  //! I use the default copy-constructor (C++11)
  /*!
    I explicitely report the default even if not strictly necessary, to 
    recall the reader that C++ provides a synthetic copy constructor.
   */
  Mesh1D(Mesh1D const&)=default;
  //! Move constructor
  /*
    Since I store the data in a vector<T> and std vectors implement
    move semantic I can just use the synthetic move constructor!
   */
  Mesh1D(Mesh1D&&)=default; //C++11
  //! I use default copy-assignement.
  /*!
    I explicitely report the default even if not strictly necessary, to 
    recall the reader that C++ provides a synthetic copy assignment.
   */
  Mesh1D & operator=(Mesh1D const &)=default;
  //! I use default move-assignement.
  /*!
    I explicitely report the default even if not strictly necessary, to 
    recall the reader that C++ provides a synthetic move assignment.
    Here I use the default because the data is stored in a vector<T> and
    std vector already support move semantic!
   */
  Mesh1D & operator=(Mesh1D&&)=default;
  
  //! Number of nodes.
  unsigned int numNodes() const { return myNodes.size();}
  //! The i-th node.
  double operator [](int i)const {return myNodes[i];}
  //! I return a copy of the DOmain1D.
  /*!
    In case it is needed.
   */
  Domain1D domain() const {return myDomain;}
  //! The mesh size.
  double h() const;
private:
  Domain1D myDomain;
  vector<double> myNodes;
};

}
#endif
