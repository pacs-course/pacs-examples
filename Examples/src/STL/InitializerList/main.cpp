/*
 * main.cpp
 *
 *  Created on: Feb 10, 2022
 *      Author: forma
 */
#include <iostream>

#include "poly.hpp"

void printPoly(apsc::Poly const & p)
{
  std::cout<<"Poly vertices:\n";
  std::cout<<"X\tY\n";
  for (auto i=0u;i<p.size();++i)
    {
      std::cout<<p[i].x<<"\t"<<p[i].y<<std::endl;
    };
  std::cout<<std::endl;
}
int main()
{
  using namespace apsc;

  // Point2D is an aggregate and Poly accepts an initializer list so I can do
  Poly p{{3.,4.},{2., 7.},{1.0,-2.0}}; // A Poly with 3 vertices
  printPoly(p);
  // I can assign a list
  p={{5.,6.},{8., 9.},{-1.0,2.0},{10.,-4.}};
  printPoly(p);
  // I can add using a init list (note the 2 brackets!)
  p.add({{0.,0.}});
  printPoly(p);
}



