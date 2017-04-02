#ifndef _HH_MESH_HH
#define _HH_MESH_HH
#include "domain.hpp"
#include <vector>

//! functions which generate a 1D mesh
namespace Generator
{

  using namespace std;
  //! Uniform function generator.
  vector<double> uniform( double const & a,
			  double const & b,
			  unsigned int const & n );
}

namespace Geometry
{

  using std::vector;

  /*!
    The splitter function defines how the mesh is generated.
    It takes as argument the left and right ends and th enumber of intervals
    (not node!). It returns the vector of nodes.
  */
  typedef vector<double> (*Splitter)( double const &,
				      double const &,
				      unsigned int const & );

  /*!
    @class Mesh1D
    This class implement the comcept of 1D mesh, i.e. a mesh of an interval.
    It stores the domain and the nodes. The values are readed using GetPot.
    The mesh can be uniform or non uniform according to the splitter function.
  */
  class Mesh1D
  {

  public:

    // Constructors and destructors.
    //! @name Constructors and destructors.
    //@{

    /*!
      Construcotr for the class.
      @param d Constant reference for the domain to be meshed.
      @param data Constat reference for a GetPot object.
      @param section Constat reference for the section to read the parameters in the datafile.
      @param Splitter Function used to mesh the domain.
    */
    Mesh1D( Domain1D const & d,
	    const GetPot& data,
	    const std::string & section = "mesh/",
	    Splitter=Generator::uniform );

    //! Virtual destructor.
    virtual ~Mesh1D ();

    //@}

    // Get methods.
    //! Get methods.
    //@{

    /*!
      Return the constant number of mesh nodes.
      @return Number of mesh nodes.
    */
    inline unsigned int numNodes() const
    {
      return M_nodes.size();
    }

    /*!
      Return the constant selected node.
      @param i Constant reference for the index node.
      @return The node selected.
    */
    inline double operator []( const int& i ) const
    {
      return M_nodes[i];
    }

    /*!
      Return the selected node.
      @param i Constant reference for the index node.
      @return The node selected.
    */
    inline double& operator []( const int& i )
    {
      return M_nodes[i];
    }

    /*!
      Return the constant domain meshed.
      @return The domain meshed.
    */
    inline Domain1D domain() const
    {
      return M_domain;
    }

    /*!
      Return the size of the mesh.
      @return The size of the mesh.
    */
    double h() const;

    //@}

  private:

    // Data values for the mesh.
    //! @name Data values for the mesh.
    //@{

    //! The domain meshed.
    const Domain1D & M_domain;

    //! The mesh as a vector of grid nodes.
    vector<double>   M_nodes;

    //@}
  };

}
#endif
