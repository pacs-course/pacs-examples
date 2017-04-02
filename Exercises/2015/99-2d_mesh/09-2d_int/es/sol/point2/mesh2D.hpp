#ifndef _MESH2D_H
#define _MESH2D_H 1
#include "GeoElem.hpp"
#include "GetPot"
#include <vector>
#include <string>

namespace Geometry
{

  /*!
    @class Mesh2D
    This class implement the comcept of 2D mesh, i.e. a mesh of a domain.
    It stores the triangles. The paramethers are readed using GetPot, while
    the mesh is generated usign Triangle.
  */
  class Mesh2D
  {

  public:

    // Constructors and destructors.
    //! @name Constructors and destructors.
    //@{

    /*!
      Construcotr for the class.
      @param data Constat reference for a GetPot object.
      @param section Constat reference for the section to read the parameters in the datafile.
    */
    Mesh2D( const GetPot& data,
	    const std::string & section = "mesh/" );

    //! Virtual destructor.
    virtual ~Mesh2D ();

    //@}

    // Get methods.
    //! Get methods.
    //@{

    /*!
      Return the constant number of mesh triangles.
      @return Number of mesh triangles.
    */
    inline unsigned int numTriangles() const
    {
      return M_triangles.size();
    }

    /*!
      Return the constant selected triangle.
      @param i Constant reference for the index triangle.
      @return The triangle selected.
    */
    inline Triangle operator []( const int& i ) const
    {
      return M_triangles[i];
    }

    /*!
      Return the selected triangle.
      @param i Constant reference for the index triangle.
      @return The triangle selected.
    */
    inline Triangle& operator []( const int& i )
    {
      return M_triangles[i];
    }

    //@}

    void generateMesh();

  private:

    // Data values for the mesh.
    //! @name Data values for the mesh.
    //@{
    const GetPot& M_data;

    const std::string M_section;

    //! The mesh as a vector of grid triangles.
    std::vector<Triangle>   M_triangles;

    //@}
  };

}
#endif
