#include <set>
#include <algorithm>
#include <iostream>
#include <array>
#include "mesh.hpp"
#include "mesh2D.hpp"
#include "mesh3D.hpp"

int main(){
    std::cout<<"2D MESH"<<std::endl<<std::endl;
    //2D mesh creation
    std::vector<Triangle> mytriangles;
    mytriangles.push_back(Triangle({{0,1,4}}));
    mytriangles.push_back(Triangle({{1,2,3}}));
    mytriangles.push_back(Triangle({{4,1,3}}));

    std::set<Edge,EdgeLessNotOrdered> alledges=GetAllEdges(mytriangles);
    std::cout<<"All Edges:"<<std::endl<<alledges<<std::endl;

    std::set<Edge,EdgeLessNotOrdered> boundedges=GetBoundaryEdges(mytriangles);
    std::cout<<"Boundary Edges:"<<std::endl<<boundedges<<std::endl;

    std::cout<<"****************************************"<<std::endl;
    std::cout<<"3D MESH"<<std::endl<<std::endl;
     //3D mesh creation
    std::vector<Tetrahedron> mytetraheda;
    mytetraheda.push_back(Tetrahedron({{0,1,2,3}}));
    mytetraheda.push_back(Tetrahedron({{0,2,4,3}}));

    std::set<Triangle,TriangleLessNotOrdered> alltriangles=GetAllTriangles(mytetraheda);
    std::cout<<"All Triangles:"<<std::endl<<alltriangles<<std::endl;

    std::set<Triangle,TriangleLessNotOrdered> boundtriangles=GetBoundaryTriangles(mytetraheda);
    std::cout<<"Boundary Triangles:"<<std::endl<<boundtriangles<<std::endl;
}
