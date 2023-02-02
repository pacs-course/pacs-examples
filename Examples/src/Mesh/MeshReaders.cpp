/*
 * MeshReaders.cpp
 *
 *  Created on: Oct 4, 2010
 *      Author: forma
 */
#include "MeshReaders.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
namespace
{
//! An helper function
void
skipInput(std::istream &f, std::string s)
{
  std::string currLine;
  do
    {
      std::getline(f, currLine);
    }
  while((currLine.find(s) == std::string::npos) && !f.eof());
}
} // namespace
namespace Fem
{
int
MeshReadSimple::read(MeshTria &m, std::string const &fileName)
{
  MeshHandler            mesh(m);
  std::vector<Point> &   pl(mesh.pointList);
  std::vector<Triangle> &el(mesh.elementList);
  std::ifstream          f;
  f.open(fileName.c_str());
  std::string currLine;
  if(!f.is_open())
    {
      std::cerr << "Mesh file does not exist or is corrupted" << std::endl;
      return 1;
    }
  // Read the number of points and elements
  if(this->M_verbose)
    std::cout << "[Mesh::Mesh] Reading mesh data" << std::endl;
  // Scan file lines
  // Skip until data or end of file
  skipInput(f, std::string("DATA"));
  // If the string is not found, abort
  if(f.eof() || f.fail())
    {
      std::cerr << "FILE ERROR! Cannot find #DATA" << std::endl;
      return 2;
    }
  // Read number of points and elements
  int nP, nE;
  f >> nP >> nE;
  if(this->M_verbose)
    std::cout << "Number of points=" << nP << " Number of elements=" << nE
              << std::endl;
  pl.reserve(nP);
  el.reserve(nE);
  // Now look for the points
  skipInput(f, std::string("POINTS"));
  // If the string is not found, abort
  if(f.eof() || f.fail() || f.bad())
    {
      std::cerr << "FILE ERROR! cannot find # POINTS" << std::endl;
      return 3;
    }

  // Fill point data structures
  for(int i = 0; i < nP; ++i)
    {
      Point  P;
      double x, y;
      int    id;
      f >> x >> y >> id;
      P[0] = x;
      P[1] = y;
      P.bcId() = id;
      // std::cout<<x<<" " <<y<<" "<< id<<" "<<pl.size()<<std::endl;
      // std::cout<<P[0]<<" " <<P[1]<<" "<< P.bcId()<<" "<<pl.size()<<std::endl;
      P.id() = pl.size();
      pl.push_back(P);
      if(f.eof() || f.fail())
        {
          std::cerr << "FILE ERROR! cannot read all point" << std::endl;
          return 3;
        }
    }
  if(this->M_verbose)
    {
      std::cout << "Points read" << std::endl;
      std::cout << "Reading elements" << std::endl;
    }
  skipInput(f, std::string("ELEMENTS"));

  // If the string is not found, abort
  if(f.eof() || f.fail())
    {
      std::cerr << "FILE ERROR! Cannot find # ELEMENTS" << std::endl;
      return 4;
    }
  // Fill element data structures
  for(int i = 0; i < nE; i++)
    {
      int type;
      f >> type;
      if(type != 0)
        {
          std::cerr << "I can read only triangular elements" << std::endl;
          return 5;
        }
      int i1, i2, i3;
      if(!f.eof())
        f >> i1 >> i2 >> i3;
      if(f.eof() || f.fail())
        {
          std::cerr << "FILE ERROR! Cannot read elements" << std::endl;
          return 5;
        }
      Triangle t(pl[i1], pl[i2], pl[i3]);
      t.id() = el.size();
      t.bcId() = 0;
      el.emplace_back(std::move(t));
    }
  f.close();
  return 0;
}

int
DummyMesh::read(MeshTria &m, std::string const &)
{
  MeshHandler mesh(m);
  mesh.pointList.reserve(4);
  mesh.pointList.push_back(Point(0, 0));
  mesh.pointList.push_back(Point(1, 0));
  mesh.pointList.push_back(Point(0, 1));
  mesh.pointList.push_back(Point(2, 2));
  for(unsigned int i = 0; i < mesh.pointList.size(); ++i)
    {
      Point &p = mesh.pointList.at(i);
      p.id() = i;
      p.bcId() = 1;
    }
  mesh.elementList.emplace_back(mesh.pointList[0], mesh.pointList[1],
                                mesh.pointList[2], 0);

  mesh.elementList.emplace_back(mesh.pointList[0], mesh.pointList[2],
                                mesh.pointList[3], 1);

  return 0;
}

int
MeshReadTriangle::read(MeshTria &m, std::string const &filename)
{
  using std::clog;
  using std::endl;
  using std::ifstream;
  using std::string;

  MeshHandler            mesh(m);
  std::vector<Point> &   pl(mesh.pointList);
  std::vector<Triangle> &el(mesh.elementList);
  /*	.node files:
First line:  <# of points> <dimension (must be 2)> <# of attributes>
<# of boundary markers (0 or 1)>
Remaining lines:  <point #> <x> <y> [attributes] [boundary marker]
The attributes, which are typically floating-point values of physical
quantities (such as mass or conductivity) associated with the nodes of
a finite element mesh, are copied unchanged to the output mesh.  If -s,
-q, or -a is selected, each new Steiner point added to the mesh will
have attributes assigned to it by linear interpolation.

If the fourth entry of the first line is `1', the last column of the
remainder of the file is assumed to contain boundary markers.
  */
  string nodeFileName(filename);
  nodeFileName += string(".node");
  ifstream nodeFile(nodeFileName.c_str());
  if(nodeFile.fail())
    {
      nodeFile.close();
      throw std::runtime_error("Cannot open triangle node file " +
                               nodeFileName);
    }
  int numPoints, dimension, numAttributes, numBMarkers;
  nodeFile >> numPoints >> dimension >> numAttributes >> numBMarkers;
  pl.resize(numPoints);
  if(M_verbose)
    std::clog << "Num points" << numPoints << std::endl;
  int    bMarker(0);
  int    nId;
  double x, y, attr;
  for(int i = 0; i < numPoints; ++i)
    {
      nodeFile >> nId >> x >> y;
      if(nodeFile.bad())
        {
          nodeFile.close();
          throw std::runtime_error("Error while reading nodes");
        }
      // We discard attributes
      for(int j = 0; j < numAttributes; ++j)
        nodeFile >> attr;
      if(numBMarkers == 1)
        nodeFile >> bMarker;
      if(nodeFile.bad())
        {
          nodeFile.close();
          throw std::runtime_error("Error while reading nodes attributes");
        }
      --nId; // Numbering from 0!
      pl[nId][0] = x;
      pl[nId][1] = y;
      pl[nId].id() = nId;
      pl[nId].bcId() = bMarker;
    }
  nodeFile.close();
  string eleFileName(filename);
  // Read elements
  eleFileName += string(".ele");
  ifstream eleFile(eleFileName.c_str());
  if(eleFile.fail())
    {
      std::cerr << "Cannot open triangle ele file " << eleFileName << endl;
      return 2;
    }
  /*   First line:  <# of triangles> <points per triangle> <# of attributes>
 Remaining lines:  <triangle #> <point> <point> <point> ... [attributes]

 Points are indices into the corresponding .node file.  The first three
 points are the corners, and are listed in counterclockwise order around
 each triangle.  (The remaining points, if any, depend on the type of
 finite element used.)  The attributes are just like those of .node
 files.*/
  int numEle, numPTria;
  eleFile >> numEle >> numPTria >> numAttributes;
  if(M_verbose)
    std::clog << "Num Elements  " << numEle << std::endl;
  el.resize(numEle);
  int i1, i2, i3, inext;
  for(int i = 0; i < numEle; ++i)
    {
      eleFile >> nId >> i1 >> i2 >> i3;
      if(eleFile.bad())
        {
          eleFile.close();
          throw std::runtime_error("Error while reading elements");
        }

      // I neglect extra points for quadratic tria
      for(int j = 3; j < numPTria; ++j)
        eleFile >> inext;
      for(int j = 0; j < numAttributes; ++j)
        eleFile >> attr;
      if(eleFile.bad())
        {
          eleFile.close();
          throw std::runtime_error("Error while reading element attributes");
        }
      --nId; // numbering from 0
      el[nId].changePoint(0, pl[--i1]);
      el[nId].changePoint(1, pl[--i2]);
      el[nId].changePoint(2, pl[--i3]);
      el[nId].id() = nId;
      el[nId].bcId() = 0;
    }
  eleFile.close();

  string edgeFileName(filename);
  // Read elements
  edgeFileName += string(".edge");
  ifstream edgeFile(edgeFileName.c_str());
  /*   First line:  <# of edges> <# of boundary markers (0 or 1)>
 Following lines:  <edge #> <endpoint> <endpoint> [boundary marker]

 Endpoints are indices into the corresponding .node file.  Triangle can
 produce .edge files (use the -e switch), but cannot read them.  The
 optional column of boundary markers is suppressed by the -B switch.*/
  if(edgeFile.fail())
    {
      std::clog << "triangle Edge file not present" << endl;
      return 0;
    }
  int numEdges;
  edgeFile >> numEdges >> numBMarkers;
  std::vector<Edge> &edl(mesh.edgeList);
  edl.resize(numEdges);
  bMarker = 0;
  for(int i = 0; i < numEdges; ++i)
    {
      edgeFile >> nId >> i1 >> i2;
      if(edgeFile.bad())
        {
          edgeFile.close();
          throw std::runtime_error("Error while reading edges");
        }

      if(numBMarkers == 1)
        edgeFile >> bMarker;
      if(edgeFile.bad())
        {
          edgeFile.close();
          throw std::runtime_error("Error while reading edge attributes");
        }
      --nId; // we start from zero!
      edl[nId].changePoint(0, pl[--i1]);
      edl[nId].changePoint(1, pl[--i2]);
      edl[nId].bcId() = bMarker;
      edl[nId].id() = nId;
    }
  edgeFile.close();
  return 0;
}

} // namespace Fem
