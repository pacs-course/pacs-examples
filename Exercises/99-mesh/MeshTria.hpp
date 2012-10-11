/*
 * MeshTria.h
 *
 *  Created on: Oct 3, 2010
 *      Author: forma
 */

#ifndef MESHTRIA_H_
#define MESHTRIA_H_
#include <vector>
#include "femMesh.hpp"
#include<string>
#include<memory>
#include<iosfwd>
namespace Fem {
class MeshTria;

//! Exposes Mesh class implementation
/*!
 * Used to allow mesh readers to access private mesh
 * data.
 */
struct MeshHandler{
    MeshHandler(MeshTria &mesh);
	std::vector<Point> & pointList;
    std::vector<Triangle> & elementList;
	std::vector<Edge> & edgeList;
	std::vector<Edge> & bEdgeList;
	MeshTria & m;
};


//! Base class for all readers
/*!
 * This class is clonable so that the Mesh class
 * may own a copy of a derived class
 */
class MeshReader{
public:
	MeshReader(){};
	virtual ~MeshReader(){};
	virtual int read(MeshTria& mesh, std::string const & filename)=0;
	virtual std::auto_ptr<MeshReader> clone()const=0;
	void setverbose(bool set=true){M_verbose=set;};
protected:
	virtual MeshReader & operator=(MeshReader const &);
	MeshReader(MeshReader const & m):M_verbose(m.M_verbose){};
protected:
	bool M_verbose;
};


class MeshTria {
public:
	typedef unsigned int size_type;
	MeshTria();
	MeshTria(MeshReader const &);
	MeshTria(const MeshTria &);
	MeshTria & operator =(MeshTria const &);
	~MeshTria(){};
	friend class MeshHandler;
    void setReader(MeshReader const &);
    size_type num_points()const {return M_pointList.size();}
    Point const & point(size_type i)const {return M_pointList[i];}
    size_type num_elements()const {return M_elementList.size();}
    Triangle const & triangle(size_type i)const {return M_elementList[i];}
    size_type num_edges()const {return M_edgeList.size();}
    Edge const & edge(size_type i)const {return M_edgeList[i];}
    size_type num_bEdges()const {return M_bEdgeList.size();}
    Edge const & bEdge(size_type i)const {return M_bEdgeList[i];}
    bool has_bEdges()const{return ! M_bEdgeList.empty();}
    int readMesh(std::string const & file);
	double measure()const;
	bool checkmesh()const;
private:
	std::vector<Point> M_pointList;
	std::vector<Triangle> M_elementList;
	std::vector<Edge> M_edgeList;
	std::vector<Edge> M_bEdgeList;
	std::auto_ptr<MeshReader> M_reader;
};

	std::ostream & operator<<(std::ostream &, MeshTria const&);
}

#endif /* MESHTRIA_H_ */
