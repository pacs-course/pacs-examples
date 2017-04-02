/*
 * femMesh.hpp
 *
 *  Created on: Oct 1, 2010
 *      Author: forma
 */

#ifndef FEMMESH_HPP_
#define FEMMESH_HPP_
#include "geo.hpp"
#include <limits>
#include <iosfwd>
#include "smallMatrix.hpp"
#include "Eigen/Dense"
namespace Fem{
typedef unsigned int Id;
typedef unsigned int BcId;
using Geometry::ndim;
using LinearAlgebra::SMatrix;
using Eigen::Matrix;
typedef Geometry::Point GeoPoint;
typedef Geometry::Triangle GeoTriangle;
// Indicates a non valid number for an identifier
const unsigned int NVAL=std::numeric_limits<unsigned int>::max();

//! Extra features for a geometric entity used in a fem code
/*!
 * This class implements the handling of boundary/domain markers
 * and entity identifiers. If the entity is stored in a linear container
 * the identifier must be equal to the position of the object
 * in the container.
 */
class FemEntity{
public:
	FemEntity():M_id(NVAL),M_bcId(NVAL){}
	bool unassignedId()const {return M_id==NVAL;}
	bool unassignedBc()const {return M_bcId==NVAL;}
	Id & id(){return M_id;}
	Id const & id()const {return M_id;}
	BcId & bcId(){return M_bcId;}
	BcId const & bcId()const {return M_bcId;}
	protected:
	Id M_id;
	BcId M_bcId;
};

class Point: public GeoPoint, public FemEntity{
public:
	explicit  Point(double x=0, double y=0);
	Point(GeoPoint const &);
};

class Edge: public FemEntity{
  public:
    static const int numVertices=2;
    static const int numSides=1;
    static const int myDim=1;
    Edge(); //Constructs an empty edge
    Edge(Point&,Point&); //Points are given (by reference)
    Point * changePoint(int i, Point &);
    // We get the points by operator [] (defined in-class for inlining)
    Point const & operator[](int i) const {return *(M_points[i]);}
    Point & operator[](int i){return *(M_points[i]);}
    double measure() const; // Edge Length
    // Returns x=x(t)
    GeoPoint map(double const &) const ;
    // dx/dt at a point t\in(0,1) returns edge length
    Eigen::Matrix<double,ndim,1> jacobian(double const &) const;
    //checks if the edge is empty: M_points contain null pointers
    bool empty()const {
    	return M_points[0]==0||M_points[1]==0;} // inlined function
 private:
	// I don't store directly a eigen matrix because of the limitations
	// of the current problems of alignement (see eigen 3.0 documentation)
	// It is not very efficient and should be changed asap
    SMatrix<Point *, numVertices, 1> M_points;
  };
//! Class for a triangular, goemetrically linear finite element.
/*!
 * I could have inherited publicly from Geomtetry::Triangle, yet since
 * I have to override most of the methods I preferred writing a simple class.
 */
class Triangle : public FemEntity {
public:
    static const int numVertices=3;
    static const int numSides=3;
    static const int myDim=2;
	Triangle(); //!Constructs an empty triangle
	Triangle(Point&,Point&,Point&);
	Point const & operator[](int i) const {return *M_points[i];}
	Point & operator[](int i){return *M_points[i];}
	// The point on an edge
	Point const & edgePoint(int edgenum,int endnum) const;
	Point * changePoint(int i, Point &p);
	// Return x=T(u,v)
	GeoPoint map(GeoPoint const & u) const;
	// Return \f$(u,v) = T^{-1}(x)$
	GeoPoint invMap(GeoPoint const & x) const;
	// The area
	double measure() const{return 0.5*detJ;};
	//\f$J=dx/d(u,v)\f$ at a point in the ref plane return det(J)
	Eigen::Matrix<double,myDim,ndim>
	jacobian(double u=0, double v=0) const;
	//\f$(d(xy)/d(uv))^{-1}\f$ at a point in the ref plane
	// \f$J_{ij}=dx_i/du_j\f$
	Eigen::Matrix<double,myDim,ndim>
	invJac(double u=0, double v=0) const;
	bool empty()const;
	// verifies orientation
	bool orientation()const{return detJ>0.0;};
	//! sets orientaton
	void setOrientation(bool positive=true);
	//! Mass Matrix (unitary density)
	Eigen::Matrix<double,3,3> localMassMatrix() const;
	//! Stiffness matrix (unitary diffusivity)
	Eigen::Matrix<double,3,3> localStiffMatrix() const;
private:
	// I don't store directly a eigen matrix because of the limitations
	// of the current problems of alignement (see eigen 3.0 documentation)
	// It is not very efficient and should be changed asap
	SMatrix<Point *, numVertices, 1> M_points;
	SMatrix<double,myDim,ndim> M_J;
	SMatrix<double,myDim,ndim> M_invJ;
	double detJ;
	void M_setJac();
};
	std::ostream & operator <<(std::ostream&, Triangle const &);

}

#endif /* FEMMESH_HPP_ */
