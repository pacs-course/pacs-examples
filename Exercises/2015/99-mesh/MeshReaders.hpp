/*
 * MeshReaders.hpp
 *
 *  Created on: Oct 4, 2010
 *      Author: forma
 */

#ifndef MESHREADERS_HPP_
#define MESHREADERS_HPP_
#include "MeshTria.hpp"
namespace Fem{
class MeshReadSimple: public Fem::MeshReader {
public:
	MeshReadSimple(){};
	int read(MeshTria & m, std::string const & filename);
	std::auto_ptr<MeshReader> clone()const;
};

class MeshReadTriangle: public Fem::MeshReader {
public:
	MeshReadTriangle(){};
	int read(MeshTria&, std::string const & filename);
	std::auto_ptr<MeshReader> clone()const;
};

class DummyMesh: public Fem::MeshReader {
public:
	DummyMesh(){};
	int read(MeshTria&, std::string const & filename);
	std::auto_ptr<MeshReader> clone()const;
};
}
#endif /* MESHREADERS_HPP_ */
