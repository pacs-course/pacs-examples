#ifndef MESH_HPP
#define MESH_HPP

#include <array>

struct Edge
{
    //Identification numbers of points representing an edge
    std::array<unsigned,2> pointId;
};

struct Triangle
{
    //Identification numbers of points representing a triangle
    std::array<unsigned,3> pointId;
};

struct Tetrahedron
{
    //Identification numbers of points representing a tetrahedron
    std::array<unsigned,4> pointId;
};

#endif
