/*
 * test_index.cpp
 *
 *  Created on: Mar 4, 2022
 *      Author: forma
 */
#include "AdtBox.hpp"
#include <iostream>
#include <list>
#include <random>
#include <vector>

#include "IndexList.hpp"
#include "AdtTree.hpp"
#include "AdtVisitors.hpp"
int
main()
{
  using namespace apsc::adt;
  std::list<std::size_t> l;
  apsc::adt::IndexList   indexList(10);
  std::cout << indexList << std::endl;
  std::cout << "getting a node" << std::endl;
  auto index = indexList.getNodeIndex();
  l.push_back(index);
  std::cout << "Taking out:" << index << std::endl;
  std::cout << indexList << std::endl;
  std::cout << "Putting back:" << index << std::endl;
  indexList.returnNodeIndex(index);
  l.remove(index);
  std::cout << indexList << std::endl;
  std::cout << "getting 8 nodes" << std::endl;
  for(auto i = 0; i < 8; ++i)
    {
      index = indexList.getNodeIndex();
      l.push_back(index);
    }
  std::cout << indexList << std::endl;
  index = *(++l.begin());
  std::cout << "Putting back index" << index << std::endl;
  indexList.returnNodeIndex(index);
  std::cout << indexList << std::endl;

  std::cout << "increase size by 4" << std::endl;
  indexList.increaseSize(4);
  std::cout << indexList << std::endl;

  Point<3>          b{3.0, 4.0, 6.0};
  Point<3>          t{6.0, 8.0, 20.5};
  apsc::adt::Box<3> boundingBox{b, t};


  std::mt19937 urbg{123};

  using ud = std::uniform_real_distribution<double>;
  std::array<std::uniform_real_distribution<double>, 3> gener{
    ud{3.0, 6.0}, ud{4.0, 8.0}, ud{6.0, 20.5}};

  {// Box Box
   using BOX=Box<1,AdtType::Point>;
   constexpr size_t DIM=1u;
  using NODE=AdtNode<BOX>;
  std::vector<NODE> vp;
  auto nbox = 100u;
  for(auto i = 0u; i < nbox; ++i)
    {
      Point<DIM> l;
      Point<DIM> u;
      for(auto j = 0u; j < DIM; ++j)
        {
          double a = gener[j](urbg);
          double b = gener[j](urbg);
          l[j] = std::min(a, b);
          u[j] = std::max(a, b);
        }
//      vp.emplace_back(l, u);
     vp.emplace_back(l);
    }
//  for(auto const &b : vp)
//    std::cout << b;

  Normaliser<BOX::dim()> normalise;
  normalise.setBoundingBox(vp);
  for(auto & b:vp)
    {
      normalise(b);
    }
  for(auto const &b : vp)
    std::cout << b;
  AdtTree<NODE> adtTree;
  for(auto & b:vp)
    adtTree.add(b);

  adtTree.visit(PrintOutVisitor{std::cout});
  Box<1,AdtType::Box> searchBox{{0.8},{0.9}};
  apsc::adt::IntersectionVisitor intersect{searchBox};
  adtTree.visit(std::ref(intersect));
  std::cout<<"intersecting "<<searchBox.corner(0)[0]<<" "<<searchBox.corner(1)[0]<<std::endl;
  for (auto i:intersect.intersectingIndexes())
    std::cout<<i<<" "<<adtTree[i].corner(0)[0]<<" "<<"\n";
  std::cout<<std::endl;
}
  {// Box Box
   using BOX=Box<2,AdtType::Box>;
   constexpr size_t DIM=2u;
   using NODE=AdtNode<BOX>;
   std::vector<NODE> vp;
   auto nbox = 200u;
   for(auto i = 0u; i < nbox; ++i)
     {
       Point<DIM> l;
       Point<DIM> u;
       for(auto j = 0u; j < DIM; ++j)
         {
           double a = gener[j](urbg);
           double b = gener[j](urbg);
           l[j] = std::min(a, b);
           u[j] = std::max(a, b);
         }
      vp.emplace_back(l, u);
//      vp.emplace_back(l);
     }
 //  for(auto const &b : vp)
 //    std::cout << b;

   Normaliser<BOX::dim()> normalise;
   normalise.setBoundingBox(vp);
   for(auto & b:vp)
     {
       normalise(b);
     }
   for(auto const &b : vp)
     std::cout << b;
   AdtTree<NODE> adtTree;
   for(auto & b:vp)
     adtTree.add(b);

   adtTree.visit(PrintOutVisitor{std::cout});
   Box<2,AdtType::Point> searchBox{{0.9,0.5}};
   apsc::adt::IntersectionVisitor intersect{searchBox};
   adtTree.visit(std::ref(intersect));
   std::cout<<"intersecting ("<<searchBox.corner(0)[0]<<" "<<searchBox.corner(0)[1]<<")"<<" ("<<searchBox.corner(1)[0]<<" "<<searchBox.corner(1)[1]<<")"<<std::endl;
   int counter{1};
   for (auto i:intersect.intersectingIndexes())

     std::cout<<"Node n:"<<counter++<<"\n"<<static_cast<BOX>(adtTree[i])<<" "<<"\n";
   std::cout<<std::endl;
 }

  /*
  std::cout<<"Deleting Node 1,2,3,4\n";
  adtTree.erase(1);
  std::cout<<"done"<<std::endl;
  adtTree.erase(3);
  std::cout<<"done"<<std::endl;
  adtTree.erase(4);
  adtTree.erase(2);
  std::cout<<"done"<<std::endl;
  adtTree.visit(PrintOutVisitor{});
 std::cout<<"Adding two Nodes \n";
  adtTree.add(vp[0]);
  adtTree.add(vp[1]);
  adtTree.visit(PrintOutVisitor{});
*/
return 0;
  }

  /*
  {// Point Box
    constexpr unsigned DIM=1;
    using BOX=AdtNode<Box<DIM,AdtType::Point>>;
   std::vector<BOX> vp;
    auto nbox = 10u;
    for(auto i = 0u; i < nbox; ++i)
      {
        Point<DIM> l;
        for(auto j = 0u; j < DIM; ++j)
          {
            double a = gener[j](urbg);
            l[j] = a;
          }
        vp.emplace_back(l);
      }
//    for(auto const &b : vp)
//      std::cout << b;

    Normaliser normalise{boundingBox};
    for(auto & b:vp)
      {
        normalise(b);
      }
   for(auto const &b : vp)
      std::cout << b;
    AdtTree<BOX> adtTree;
    for(auto & b:vp)
      adtTree.add(b);

    adtTree.visit(PrintOutVisitor{});
    std::cout<<"Deleting Node 3\n";
    adtTree.erase(3);
    adtTree.visit(PrintOutVisitor{});
    std::cout<<"Adding a Node \n";
    adtTree.add(vp[0]);
    adtTree.visit(PrintOutVisitor{});


  }

  NodeControl<6> controller;


}*/
