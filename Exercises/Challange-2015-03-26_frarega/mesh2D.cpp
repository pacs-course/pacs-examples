#ifndef MESH2D_CPP
#define MESH2D_CPP

#include <set>
#include <algorithm>
#include <iostream>
#include <vector>
#include <array>
#include "mesh.hpp"
#include "mesh2D.hpp"

bool EdgeLessNotOrdered::operator ()(Edge const& a, Edge const& b)
{
    	auto amax=std::max(a.pointId[0],a.pointId[1]);
        auto bmax=std::max(b.pointId[0],b.pointId[1]);
        if (amax != bmax) return amax<bmax;

        auto amin=std::min(a.pointId[0],a.pointId[1]);
        auto bmin=std::min(b.pointId[0],b.pointId[1]);
        return amin<bmin;
}

std::ostream & operator << (std::ostream &os, std::set<Edge,EdgeLessNotOrdered>& edges) 
{
    for (auto i: edges)
        std::cout<<i.pointId[0]<<" "<<i.pointId[1]<<std::endl;
    return os;
}

std::set<Edge,EdgeLessNotOrdered> GetAllEdges(std::vector<Triangle> &trs)
{
    std::set<Edge,EdgeLessNotOrdered> edges;
    for (Triangle t: trs)
    {
        edges.insert(Edge({{t.pointId[0],t.pointId[1]}}));
        edges.insert(Edge({{t.pointId[1],t.pointId[2]}}));
        edges.insert(Edge({{t.pointId[2],t.pointId[0]}}));
    }
    return edges;
}

template<class Compare>
void SelectiveInsert(std::set<Edge,Compare> &edges, unsigned const &a, unsigned const &b)
{
    auto ins = edges.insert(Edge({{a,b}}));
    if (!ins.second) edges.erase(ins.first);
}

std::set<Edge,EdgeLessNotOrdered> GetBoundaryEdges(std::vector<Triangle> const &trs)
{
    std::set<Edge,EdgeLessNotOrdered> edges;
    for (Triangle t: trs)
    {
        SelectiveInsert(edges,t.pointId[0],t.pointId[1]);
        SelectiveInsert(edges,t.pointId[1],t.pointId[2]);
        SelectiveInsert(edges,t.pointId[2],t.pointId[0]);
    }
    return edges;
}

#endif
