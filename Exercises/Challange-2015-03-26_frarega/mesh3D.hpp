#ifndef MESH3D_HPP
#define MESH3D_HPP

#include <set>
#include <algorithm>
#include <iostream>
#include <array>
#include <vector>
#include "mesh.hpp"

struct TriangleLessNotOrdered
{
    //Says whether a is less than b, considering (a,b,c), (a,c,b), (c,b,a), ecc being equivalent
    bool operator ()(Triangle const& a, Triangle const& b);
};

//Prints a list of triangles
std::ostream & operator << (std::ostream &os, std::set<Triangle,TriangleLessNotOrdered>& trs);

//Returns a set containing all the triangles of the mesh made of the list of tetrahedra "trs", with no repetition.
std::set<Triangle,TriangleLessNotOrdered> GetAllTriangles(std::vector<Tetrahedron> &tetras);

//Inserts a new triangle in the set "trs". If the triangle is already in the set, the new triangle is not insered and the triangle edge is removed.
template<class Compare>
void SelectiveInsert(std::set<Triangle,Compare> &trs, unsigned const &a, unsigned const &b, unsigned const &c);

//Returns a set containing all the BOUNDARY triangles of the mesh made of the list of tetrahedrons "trs", with no repetition.
//The idea is that a boundary triangle belongs just to uno tetrahedron, while an inner triangle belongs exactly to two tetrahedra.
//The function doesn't take into accout the orientation of triangles, so that can be used also when the triangles are not oriented.
std::set<Triangle,TriangleLessNotOrdered> GetBoundaryTriangles(std::vector<Tetrahedron> const &tetras);

#endif
