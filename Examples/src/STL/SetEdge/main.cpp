#include "Edges.hpp"
#include <set>
#include <iostream>
#include <vector>
#include <algorithm> // for set_union
#include <iterator>  // for inserter
int main()
{
  std::vector<Edge> myedges;
  myedges.emplace_back(1,2);
  myedges.emplace_back(2,1);
  myedges.emplace_back(3,4);
  myedges.emplace_back(4,3);
  std::set<Edge,yesOrient> oEdges(myedges.begin(),myedges.end());
  std::set<Edge,noOrient> noEdges(myedges.begin(),myedges.end());
  std::cout<<" with orientation"<<std::endl;
  for (auto i : oEdges)
    {
      std::cout<<i[0]<<" "<<i[1]<<std::endl;
    }
  std::cout<<" without orientation"<<std::endl;
  for (auto i : noEdges)
    {
      std::cout<<i[0]<<" "<<i[1]<<std::endl;
    }
// an example of algorithm
  std::set<Edge,yesOrient> newEdges={{1,2},{7,9}};
  std::set<Edge,yesOrient> allEdges;
// Note the use of inserter, it is necessary!
// It is also necessary to give the object containing the
// comparison operator to set_union. 
// It is not required if you have specialised
// less<T> or overloaded operator < for Edges.
//
  std::set_union(oEdges.begin(),oEdges.end(),
                 newEdges.begin(),newEdges.end(),
                 std::inserter(allEdges,allEdges.begin()),
                 yesOrient()); // here the comparison object
  std::cout<<" all edges after union (with orient.)"<<std::endl;
  for (auto i : allEdges)
    {
      std::cout<<i[0]<<" "<<i[1]<<std::endl;
    }




}
