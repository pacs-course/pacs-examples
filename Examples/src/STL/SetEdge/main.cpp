#include "Edges.hpp"
#include <algorithm> // for set_union
#include <iostream>
#include <iterator> // for inserter
#include <set>
#include <vector>
int
main()
{
  std::vector<Edge> myedges;
  myedges.emplace_back(1, 2);
  myedges.emplace_back(2, 1);
  myedges.emplace_back(3, 4);
  myedges.emplace_back(4, 3);
  std::set<Edge, yesOrient> oEdges(myedges.begin(), myedges.end());
  std::set<Edge, noOrient>  noEdges(myedges.begin(), myedges.end());
  std::cout << " with orientation" << std::endl;
  for(auto i : oEdges)
    {
      std::cout << i[0] << " " << i[1] << std::endl;
    }
  std::cout << " without orientation" << std::endl;
  for(auto i : noEdges)
    {
      std::cout << i[0] << " " << i[1] << std::endl;
    }
  // an example of algorithm
  std::set<Edge, yesOrient> newEdges = {{1, 2}, {7, 9}};
  std::set<Edge, yesOrient> allEdges;
  // Note the use of inserter, it is necessary!
  // It is also necessary to give the object containing the
  // comparison operator to set_union.
  // It is not required if you have specialised
  // less<T> or overloaded operator < for Edges.
  //
  std::set_union(oEdges.begin(), oEdges.end(), newEdges.begin(), newEdges.end(),
                 std::inserter(allEdges, allEdges.begin()),
                 yesOrient()); // here the comparison object
  std::cout << " all edges after union (with orient.)" << std::endl;
  for(auto i : allEdges)
    {
      std::cout << i[0] << " " << i[1] << std::endl;
    }

  // A trivial mesh with just triangles... 2 triangles meshing a square with
  // vertices numbered 1,2,3,4
  std::vector<Triangle> mesh;

  // this is a trivial mesh used only for example, but the set of operations is
  // the same in complex meshes
  //
  //             4......3
  //              .   /.
  //              .  / .
  //              . /  .
  //             1./....2
  mesh.emplace_back(1, 2, 3);
  mesh.emplace_back(1, 3, 4);
  // finding the edges
  std::set<Edge, noOrient> allMeshEdges;
  std::set<Edge, noOrient> internalMeshEdges;
  for(auto t : mesh) // loop over mesh triangles
    {
      // Here auto is safe
      auto tEdges = t.edges(); // get triangle edges
      for(unsigned int i = 0; i < 3; ++i)
        {
          // structured bindings. Also here it is safe
          auto [it, inserted] = allMeshEdges.insert(tEdges[i]);
          // if not inserted it exists already, so it is internal!
          if(!inserted)
            internalMeshEdges.insert(tEdges[i]);
        }
    }
  // Find boundary edges
  std::set<Edge, noOrient> boundaryMeshEdges;
  // I need to give the comparison operator since it is special!
  std::set_difference(
    allMeshEdges.begin(), allMeshEdges.end(), internalMeshEdges.begin(),
    internalMeshEdges.end(),
    std::inserter(boundaryMeshEdges, boundaryMeshEdges.begin()), noOrient{});

  std::cout << " Mesh Information\n N. of Triangles " << mesh.size()
            << std::endl;
  std::cout << " All Edges:\n";
  for(auto ed : allMeshEdges)
    std::cout << ed[0] << ", " << ed[1] << std::endl;
  std::cout << " Internal Edges:\n";
  for(auto ed : internalMeshEdges)
    std::cout << ed[0] << ", " << ed[1] << std::endl;
  std::cout << " Boundary Edges:\n";
  for(auto ed : boundaryMeshEdges)
    std::cout << ed[0] << ", " << ed[1] << std::endl;
}
