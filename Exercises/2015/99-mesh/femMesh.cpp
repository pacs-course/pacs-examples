/*
 * femMesh.cpp
 *
 *  Created on: Oct 1, 2010
 *      Author: forma
 */
#include "femMesh.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <sstream>
namespace{
std::string value(Fem::Id i){
	std::ostringstream out;
	if (i==Fem::NVAL) out<<" NULL";
	else out<<i;
	return out.str();
}
}

namespace Fem{
Point::Point(double x, double y):GeoPoint(x,y),FemEntity(){}

Point::Point(const GeoPoint & p):
				GeoPoint(p),FemEntity()
{}

/*Point & Point::operator =(const Point & p)
{
	if(this != &p){
		this->GeoPoint::operator=(p);
		M_id=p.M_id;
		M_bcId=p.M_bcId;
	}
	return *this;
}*/

double Fem::Edge::measure() const{
	GeoPoint Diff(M_points[1]-M_points[0]);
	return std::sqrt(Diff[0]*Diff[0]+Diff[1]*Diff[1]);
}

Edge::Edge(Point & a, Point & b):FemEntity()
{
	M_points[0]=&a;
	M_points[1]=&b;
}

Edge::Edge():FemEntity()
{
	M_points[0]=M_points[1]=0;
}

GeoPoint Edge::map(const double & t) const
{
	GeoPoint & a = *M_points[0];
	GeoPoint & b = *M_points[1];
	return GeoPoint(a * (1 - t) + b * t);
}

Matrix<double,ndim,1> Edge::jacobian(const double & t) const
{
	GeoPoint Diff(*M_points[1] - *M_points[0]);
	return Matrix<double,ndim,1>(Diff[0],Diff[1]);
}

Point *Edge::changePoint(int i, Point & p)
{
	Point *tmp(M_points[i]);
	M_points[i] = &p;
	return tmp;
}

Triangle::Triangle()
:FemEntity(), detJ(0)
{
	for(int i = 0;i < numVertices;++i)M_points(i)=0;
	for(int i = 0;i < ndim;++i)
		for(int j = 0;j <  myDim;++j)
			M_J(j,i) = M_invJ(j,i) = 0;
}

Triangle::Triangle(Point & a, Point & b, Point & c):
		FemEntity()
{
	M_points(0)=&a;
	M_points(1)=&b;
	M_points(2)=&c;
	this->M_setJac();
	this->setOrientation();
}

bool Triangle::empty()const{
 return M_points[0]==0||M_points[1]==0 || M_points[2]==0;
}

Point *Triangle::changePoint(int i, Point & p)
{
	Point *tmp(M_points(i));
	M_points(i) = &p;
	if(!this->empty()){
		this->M_setJac();
		this->setOrientation();
	}
	return tmp;
}

Point const & Triangle::edgePoint(int edgenum, int endnum)const
{
	return *M_points(GeoTriangle::edge(edgenum, endnum));
}


GeoPoint Triangle::map(GeoPoint const & u) const
{
	Triangle const & t = *this;
	return GeoPoint(t[0][0] + M_J(0,0) * u[0] + M_J(0,1) * u[1],
			t[0][1] + M_J(1,0) * u[0] + M_J(1,1) * u[1]);
}

GeoPoint Triangle::invMap(GeoPoint const & x) const
{
	Triangle const & t = *this;
	GeoPoint d(x-t[0]);
	return GeoPoint(M_invJ(0,0) * d[0] + M_invJ(0,1) * d[1],
			M_invJ(1,0) * d[0] + M_invJ(1,1) * d[1]);
}

Eigen::Matrix<double,Triangle::myDim,ndim>
Triangle::jacobian(double u, double v)
const{
	Eigen::Matrix<double,myDim,ndim> tmp;
	tmp(0,0)=M_J(0,0);
	tmp(1,0)=M_J(1,0);
	tmp(0,1)=M_J(0,1);
	tmp(1,1)=M_J(1,1);
	return tmp;
}

Eigen::Matrix<double,Triangle::myDim,ndim>
Triangle::invJac(double u, double v) const {
	Eigen::Matrix<double,myDim,ndim> tmp;
	tmp(0,0)=M_invJ(0,0);
	tmp(1,0)=M_invJ(1,0);
	tmp(0,1)=M_invJ(0,1);
	tmp(1,1)=M_invJ(1,1);
	return tmp;
}

void Triangle::setOrientation(bool positive)
{
	if (this->orientation()!=positive){
		std::swap(M_points[1],M_points[2]);
		std::swap(M_J(0,0),M_J(0,1));
		std::swap(M_J(1,0),M_J(1,1));
		std::swap(M_invJ(0,0),M_invJ(1,0));
		std::swap(M_invJ(0,1),M_invJ(1,1));
		detJ=-detJ;
	}
}

void Triangle::M_setJac()
{
	Triangle & t = *this;
	GeoPoint d1 = t[1] - t[0];
	GeoPoint d2 = t[2] - t[0];
	M_J(0,0) = d1[0];
	M_J(1,0) = d1[1];
	M_J(0,1) = d2[0];
	M_J(1,1) = d2[1];
	detJ = M_J(0,0) * M_J(1,1) - M_J(1,0) * M_J(0,1);
	double idet = 1. / detJ;
	M_invJ(0,0) =  idet * M_J(1,1);
	M_invJ(1,0) = -idet * M_J(1,0);
	M_invJ(0,1) = -idet * M_J(0,1);
	M_invJ(1,1) =  idet * M_J(0,0);
}
Eigen::Matrix<double,3,3>
Triangle::localMassMatrix() const{
	double area=this->measure();
	double const diag(area/6.0);
	double const off(area/12.0);
	// This is not the best way to construct the matrix
	// but it is the simplest
	Eigen::Matrix<double,3,3> a;
	a<<diag,off,off,
	   off,diag,off,
	   off,off,diag;
	return a;
}

Eigen::Matrix<double,3,3>
Triangle::localStiffMatrix() const{
	Eigen::Matrix<double,3,2> n;
	Triangle const & t(*this);
	double meas2=1/(2.0*t.measure());
	for (int i=0;i<3;++i){
		int j =  (i+1)% 3;
		int k = (j+1) % 3;
		n.row(i)<< (-t[k][1]+t[j][1]),(t[k][0]-t[j][0]);
		}
	n*=meas2;
	double off01=n.row(0).dot(n.row(1));
	double off02=n.row(0).dot(n.row(2));
	double off12=n.row(1).dot(n.row(2));
	Eigen::Matrix<double,3,3> s;
	s<<-(off01+off02),off01,off02,
			off01,-(off01+off12),off12,
			off02,off12,-(off02+off12);
	return s;
}


std::ostream & operator <<(std::ostream& out, Triangle const & t){
	out<<"** TRIANGLE ID "<<value(t.id())<<std::endl;
	out<<"Boundary condition id:"<< value(t.bcId())<<std::endl;
	out<<"Points:"<<std::endl;
	for (int i=0;i<Triangle::numVertices;++i)
		out<<t[i].id()<<": "<<t[i][0]<<" "<<t[i][1]<<std::endl;
	out<<"Jacobian of the map reference -> current"<<std::endl;
	out<<t.jacobian(0,0)<<std::endl;
	out<<"Inverse Jacobian of the map reference -> current"<<std::endl;
	out<<t.invJac(0,0)<<std::endl;
	out<<" ELEMENTAL MASS MATRIX"<<std::endl;
	out<<t.localMassMatrix()<<std::endl;
	out<<" ELEMENTAL STIFFNESS MATRIX"<<std::endl;
	out<<t.localStiffMatrix()<<std::endl;
	out<<"Triangle Edges:"<<std::endl;
	for (int i=0;i<Triangle::numSides;++i)
		out<<t.edgePoint(i,0).id()<<" "<<t.edgePoint(i,1).id()<<std::endl;
	return out;
}

}
