#include "SegmentIntersect.hpp"
#include "EdgeGeo.hpp"

int main()
{
  using namespace apsc::Geometry::Intersection;
  using apsc::Geometry::EdgeGeo;
  {
    std::array<double,2> p1={0.0,-1.0};
    std::array<double,2> p2={0.0,2.0};
    std::array<double,2> p3={0.,2.0};
    std::array<double,2> p4={0.0,0.0};
    
    EdgeGeo a(p1,p2);
    EdgeGeo b(p3,p4);
    auto intersection=segmentIntersect(a,b);
    std::cout<<intersection;
    std::cout<<std::endl<<"---------------"<<std::endl;
  }

  {
    std::array<double,2> p1={-1,-1.0};
    std::array<double,2> p2={1.,2.0};
    std::array<double,2> p3={0,-2.0};
    std::array<double,2> p4={1,10.0};
    
    EdgeGeo a(p1,p2);
    EdgeGeo b(p3,p4);
    auto intersection=segmentIntersect(a,b);
    std::cout<<intersection;
    std::cout<<std::endl<<"---------------"<<std::endl;
  }
  
 {
    std::array<double,2> p1={-1,-1.0};
    std::array<double,2> p2={1.,1.0};
    std::array<double,2> p3={1, 2.0};
    std::array<double,2> p4={-1,0.0};
    
    EdgeGeo a(p1,p2);
    EdgeGeo b(p3,p4);
    auto intersection=segmentIntersect(a,b);
    std::cout<<intersection;
    std::cout<<std::endl<<"---------------"<<std::endl;
  }

  
}
