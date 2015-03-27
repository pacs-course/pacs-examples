#ifndef MESH2D_HPP
#define MESH2D_HPP

#include <set>
#include <algorithm>
#include <iostream>
#include <array>
#include <vector>
#include "mesh.hpp"

struct EdgeLessNotOrdered
{
    //Says whether a is less than b, considering (a,b) and (b,a) being equivalent
    bool operator ()(Edge const& a, Edge const& b);
};

//Prints a list of edges
std::ostream & operator << (std::ostream &os, std::set<Edge,EdgeLessNotOrdered>& edges);

//Returns a set containing all the edges of the mesh made of the list of triangles "trs", with no repetition.
std::set<Edge,EdgeLessNotOrdered> GetAllEdges(std::vector<Triangle> &trs);

//Inserts a new edge in the set "edges". If the edge is already in the set, the new edge is not insered and the existing edge is removed.
template<class Compare>
void SelectiveInsert(std::set<Edge,Compare> &edges, unsigned const &a, unsigned const &b);

//Returns a set containing all the BOUNDARY edges of the mesh made of the list of triangles "trs", with no repetition.
//The idea is that a boundary edge belongs just to uno triangle, while an inner edge belongs exactly to two triangles.
//The function doesn't take into accout the orientation of edges, so that can be used also when the edges are not oriented.
std::set<Edge,EdgeLessNotOrdered> GetBoundaryEdges(std::vector<Triangle> const &trs);

#endif
