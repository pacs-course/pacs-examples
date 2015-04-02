#ifndef MESH3D_CPP
#define MESH3D_CPP

#include <set>
#include <algorithm>
#include <iostream>
#include <array>
#include <vector>
#include "mesh.hpp"
#include "mesh3D.hpp"

bool TriangleLessNotOrdered::operator ()(Triangle const& a, Triangle const& b)
{
        auto amax=*std::max_element(a.pointId.begin(),a.pointId.end());
        auto bmax=*std::max_element(b.pointId.begin(),b.pointId.end());
        if (amax != bmax) return amax<bmax;

        auto amin=*std::min_element(a.pointId.begin(),a.pointId.end());
        auto bmin=*std::min_element(b.pointId.begin(),b.pointId.end());
        auto amid=a.pointId[0]+a.pointId[1]+a.pointId[2]-amin-amax;
        auto bmid=b.pointId[0]+b.pointId[1]+b.pointId[2]-bmin-bmax;
        if (amid != bmid) return amid<bmid;
        return amin<bmin;
}

std::ostream & operator << (std::ostream &os, std::set<Triangle,TriangleLessNotOrdered>& trs) {
    for (auto i: trs)
        std::cout<<i.pointId[0]<<" "<<i.pointId[1]<<" "<<i.pointId[2]<<std::endl;
    return os;
}

std::set<Triangle,TriangleLessNotOrdered> GetAllTriangles(std::vector<Tetrahedron> &tetras)
{
    std::set<Triangle,TriangleLessNotOrdered> trs;
    for (Tetrahedron t: tetras)
    {
        trs.emplace({{t.pointId[0],t.pointId[1],t.pointId[2]}});
        trs.insert(Triangle({{t.pointId[0],t.pointId[2],t.pointId[3]}}));
        trs.insert(Triangle({{t.pointId[0],t.pointId[3],t.pointId[1]}}));
        trs.insert(Triangle({{t.pointId[1],t.pointId[3],t.pointId[2]}}));
    }
    return trs;
}

template<class Compare>
void SelectiveInsert(std::set<Triangle,Compare> &trs, unsigned const &a, unsigned const &b, unsigned const &c)
{
    auto ins = trs.insert(Triangle({{a,b,c}}));
    if (!ins.second) trs.erase(ins.first);
}

std::set<Triangle,TriangleLessNotOrdered> GetBoundaryTriangles(std::vector<Tetrahedron> const &tetras)
{
    std::set<Triangle,TriangleLessNotOrdered> trs;
    for (Tetrahedron t: tetras)
    {
        SelectiveInsert(trs,t.pointId[0],t.pointId[1],t.pointId[2]);
        SelectiveInsert(trs,t.pointId[0],t.pointId[2],t.pointId[3]);
        SelectiveInsert(trs,t.pointId[0],t.pointId[3],t.pointId[1]);
        SelectiveInsert(trs,t.pointId[1],t.pointId[3],t.pointId[2]);
    }
    return trs;
}

#endif
