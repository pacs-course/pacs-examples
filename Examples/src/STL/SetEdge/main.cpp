#include "Edges.hpp"
#include <set>
#include <iostream>
#include <vector>
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
}
