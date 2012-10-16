/*
 * MeshTria.cpp
 *
 *  Created on: Oct 4, 2010
 *      Author: forma
 */
#include "MeshTria.hpp"
#include <fstream>
#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>
namespace Fem{

MeshReader & MeshReader::operator=(MeshReader const & m){
	if(&m!=this){
		M_verbose=m.M_verbose;
	}
	return *this;
}


MeshHandler::MeshHandler(MeshTria &mesh):
		pointList(mesh.M_pointList),elementList(mesh.M_elementList),
		edgeList(mesh.M_edgeList),bEdgeList(mesh.M_bEdgeList),m(mesh)
{}

MeshTria::MeshTria(){}

MeshTria::MeshTria(const MeshTria & m):M_pointList(m.M_pointList){
    this->M_bEdgeList.reserve(m.M_bEdgeList.size());
	this->M_edgeList.reserve(m.M_edgeList.size());
	this->M_elementList.reserve(m.M_elementList.size());
	std::vector<Point*> old2new;
	old2new.reserve(m.num_points());

	for (unsigned int i =0;i<m.num_points();++i){
		Point * pnew = &(this->M_pointList[i]);
		old2new[i]=pnew;
	}

	for (unsigned int i=0;i<m.num_elements();++i){
		Triangle t=m.M_elementList[i];
		for (unsigned int j=0; j<3;++j)
			t.changePoint(j,*old2new[t[j].id()]);
		this->M_elementList.push_back(t);
	}
	for (unsigned int i=0;i<m.num_edges();++i){
		Edge e=m.M_edgeList[i];
		for (unsigned int j=0; j<2;++j)
			e.changePoint(j,*old2new[e[j].id()]);
		this->M_edgeList.push_back(e);
	}
	for (unsigned int i=0;i<m.num_bEdges();++i){
		Edge e=m.M_bEdgeList[i];
		for (unsigned int j=0; j<2;++j)
			e.changePoint(j,*old2new[e[j].id()]);
		this->M_bEdgeList.push_back(e);
	}
}

MeshTria & MeshTria::operator =(MeshTria const & m){
	if (&m==this) return *this;

	// Useful references
	std::vector<Point>& pointList=this->M_pointList;
	std::vector<Triangle> & elementList=this->M_elementList;
	std::vector<Edge>& edgeList=this->M_edgeList;
	std::vector<Edge>& bEdgeList=this->M_bEdgeList;

	// Destroy old data
	std::vector<Point>().swap(pointList);
	std::vector<Triangle>().swap(elementList);
	std::vector<Edge>().swap(edgeList);
	std::vector<Edge>().swap(bEdgeList);

	// Prepare for new data
	pointList.reserve(m.M_pointList.size());
	edgeList.reserve(m.M_edgeList.size());
	elementList.reserve(m.M_elementList.size());
	bEdgeList.reserve(m.M_bEdgeList.size());

	// Copy point data
	std::copy(m.M_pointList.begin(),m.M_pointList.end(),
			std::back_inserter(pointList));

	// Fix pointers
	std::vector<Point*> old2new;
	old2new.reserve(m.num_points());
	for (unsigned int i =0;i<m.num_points();++i){
		Point * pnew = &(pointList[i]);
		old2new[i]=pnew;
	}

	for (unsigned int i=0;i<m.num_elements();++i){
		Triangle t=m.M_elementList[i];
		for (unsigned int j=0; j<3;++j)
			t.changePoint(j,*old2new[t[j].id()]);
		elementList.push_back(t);
	}

	for (unsigned int i=0;i<m.num_edges();++i){
		Edge e=m.M_edgeList[i];
		for (unsigned int j=0; j<2;++j)
			e.changePoint(j,*old2new[e[j].id()]);
		edgeList.push_back(e);
	}

	for (unsigned int i=0;i<m.num_bEdges();++i){
		Edge e=m.M_bEdgeList[i];
		for (unsigned int j=0; j<2;++j)
			e.changePoint(j,*old2new[e[j].id()]);
		bEdgeList.push_back(e);
	}
	return *this;
}

MeshTria::MeshTria(MeshReader const & r):M_reader(r.clone())
{
}

void  MeshTria::setReader(MeshReader const & r){
	M_reader.release();
	M_reader.operator =(r.clone());
}


int MeshTria::readMesh(const std::string & file){
	if (this->M_reader.get() == 0){
		std::cerr<<"Mesh reader not set"<<std::endl;
		exit(1);
	}else
		return M_reader->read(*this,file);
}

double MeshTria::measure()const {
	double mes(0);
	for (std::vector<Triangle>::const_iterator i=M_elementList.begin();
			i!=M_elementList.end();++i)mes+=i->measure();
	return mes;
}

bool MeshTria::checkmesh() const{
	using std::clog;
	using std::endl;
	using std::cerr;
	bool status;
	clog<<"Mesh stores: "<<endl;
	clog<<this->num_elements()<<" Elements"<<endl;
	clog<<this->num_points()<<" Points"<<endl;
	clog<<this->num_edges()<<" Edges"<<endl;
	clog<<this->num_bEdges()<<" Boundary Edges"<<endl;
	int count(0);
	int bccount(0);
	int wrongid(0);
	unsigned int idcount(0);
	for(std::vector<Point>::const_iterator i=M_pointList.begin();
			i!=M_pointList.end(); ++i)
	{
		if (i->unassignedId())++count;
		if (i->unassignedBc())++bccount;
		if (i->id()!=idcount++)++wrongid;

	}
	clog<<"Mesh has "<<count<<" unassigned point id and "<<
			bccount<<" unassigned point bc markers"<<endl;
	status=wrongid>0;
	if(wrongid>0)cerr<<"Mesh has "<<wrongid<<" wrong point id set";
	count=0;
	bccount=0;
	wrongid=0;
	idcount=0;
	int punset(0);
	for(std::vector<Triangle>::const_iterator i=M_elementList.begin();
			i!=M_elementList.end(); ++i)
	{
		if (i->unassignedId())++count;
		if (i->unassignedBc())++bccount;
		if (i->id()!=idcount++)++wrongid;
		if (i->empty())++punset;
	}
	clog<<"Mesh has "<<count<<" unassigned element id and "<<
			bccount<<" unassigned element bc markers"<<endl;
	status=status||wrongid>0||punset>0;
	if(wrongid>0)cerr<<"Mesh has "<<wrongid<<" wrong element id set";
	if(punset>0)cerr<<"Mesh has "<<punset<<" points unset";
	if(!status)clog<<"Domain area:"<<this->measure()<<endl;
	return status;
}

std::ostream & operator<<(std::ostream & out, MeshTria const& m){
	out<< " ***** MESH  INFORMATION ******"<<std::endl;
	out<<" Num Points="<<m.num_points()<<" "<<" Num elements="<<m.num_elements()<<" "
			<<"Num. edges="<<m.num_edges()<<" "<<"Num Boundary Edges="
			<<m.num_bEdges()<<std::endl;
	out<< "POINTS:"<<std::endl;
	int oprec=out.precision(10);
	std::ios_base::fmtflags oflags=
			out.setf(std::ios_base::scientific,std::ios_base::floatfield);
	for (unsigned int i=0;i<m.num_points();++i){
		Point p=m.point(i);
		double x=p[0];
		double y=p[1];
		out<<i<<" "<<x<<" "<<y<<std::endl;
	}
	out<<" TRIANGLE CONNECTIVITY AND AREA:"<<std::endl;
	for (unsigned int i=0; i<m.num_elements();++i){
		Triangle t=m.triangle(i);
		out<<i<<" "<<t[0].id()<<" "<<t[1].id()<<" "<<t[2].id()<<
		  " "<<t.measure()<<std::endl;
	}
	out.precision(oprec);
	out.flags(oflags);
	return out;
}
}
