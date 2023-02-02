/*
 * MeshReaders.hpp
 *
 *  Created on: Oct 4, 2010
 *      Author: forma
 */

#ifndef MESHREADERS_HPP_
#define MESHREADERS_HPP_
#include "MeshTria.hpp"
namespace Fem
{
class MeshReadSimple : public Fem::MeshReader
{
public:
  MeshReadSimple(bool verbose = false) : MeshReader(verbose){};
  int read(MeshTria &m, std::string const &filename) override;
};

class MeshReadTriangle : public Fem::MeshReader
{
public:
  MeshReadTriangle(bool verbose = false) : MeshReader(verbose){};
  int read(MeshTria &, std::string const &filename) override;
};

class DummyMesh : public Fem::MeshReader
{
public:
  DummyMesh(bool verbose = false) : MeshReader(verbose){};
  int read(MeshTria &, std::string const &filename) override;
};
} // namespace Fem
#endif /* MESHREADERS_HPP_ */
